#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS_TEMP 17
#define PIN_SCK  18
#define PIN_MOSI 19


#define EEPROM_ADDR 0x50

#define CMD_READ_TEMP 0x00
#define MAX31725_ADDR 0x48

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

#define UART_ID uart0
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

// Placeholder is 0 and 1 but is probably something else
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define ATTENUATOR_1 0X40
#define ATTENUATOR_2 0X42
#define ATTENUATOR_3 0X44
#define ATTENUATOR_4 0X46
#define ATTENUATOR_5 0X48

#define ATTENUATOR_REG_OUT 0x01

#include "blink.pio.h"

static int chars_rxed = 0;

void blink_pin_forever(PIO pio, uint sm, uint offset, uint pin, uint freq) {
    blink_program_init(pio, sm, offset, pin);
    pio_sm_set_enabled(pio, sm, true);

    printf("Blinking pin %d at %d Hz\n", pin, freq);

    // PIO counter program takes 3 more cycles in total than we pass as
    // input (wait for n + 1; mov; jmp)
    pio->txf[sm] = (125000000 / (2 * freq)) - 3;
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    // Put your timeout handler code in here
    return 0;
}

// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        // Can we send it back?
        if (uart_is_writable(UART_ID)) {
            // Change it slightly first!
            ch++;
            uart_putc(UART_ID, ch);
        }
        chars_rxed++;
    }
}

float read_adc()
{
    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();
    printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
    sleep_ms(2);
    return result;
}

/**
 * @brief read the max31725 temperature and return the temperature in Celsius
 * 
 * @param spi
 * @return float 
 */
float read_max31725_temp(i2c_inst_t *i2c)
{
    uint8_t rx_data[2];

    // Send command and read 2 bytes of data
    i2c_write_blocking(i2c, MAX31725_ADDR, CMD_READ_TEMP, 1, false);
    i2c_read_blocking(i2c, MAX31725_ADDR, rx_data, 2, false);

    int16_t temp_data = (rx_data[1] << 8) | rx_data [2];
    float temp_degC (temp_data / 256.0);

    return temp_degC;
}

void eeprom_read(i2c_inst_t *i2c, uint32_t address,  uint8_t *data, size_t len)
{
    uint8_t addr_buf[2] = {(address >> 8) & 0xFF, address & 0xFF};

    i2c_write_blocking(i2c, EEPROM_ADDR, addr_buf, 2, true);
    i2c_read_blocking(i2c, EEPROM_ADDR, data, len, false);
}

void pca9554_set_outputs(i2c_inst_t *i2c, uint8_t value)
{
    uint8_t data[2] = { ATTENUATOR_REG_OUT , value};
    i2c_write_blocking(i2c, ATTENUATOR_1, data, 2, false);
}

/**
 * @brief read the lm50 temperature and return the temperature in Celsius
 * 
 * @param V 
 * @return float 
 */
float lm50_V_to_degC(float V)
{
    // V(o) = 10mV/C * TempC + 500mV
    // (V(o) - 500mV) / 10mV = Temp

    float temp_degC = (V - 0.5) / 0.01;
    return temp_degC;
}

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1'000'000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    gpio_init(PIN_CS_TEMP);
    gpio_set_dir(PIN_CS_TEMP, GPIO_OUT);
    
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // ADC initialisation
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    // Set up our UART with the required speed.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(UART_ID, false, false);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(UART_ID, false);

    // Set up a RX interrupt
    // We need to set up the handler first
    // Select correct interrupt for the UART we are using
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;

    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    // Timer code - This fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);

    pca9554_set_outputs(i2c0, 0x00);

    // Watchdog restart code
    if (watchdog_caused_reboot()) {
        uart_puts(UART_ID, "Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }
    
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(100, 1);
    
    // You need to call this function at least more often than the 100ms in the enable call to prevent a reboot
    watchdog_update();

    char* to_send = "System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys);
    uart_puts(UART_ID, to_send);

    while (true) {
        uart_puts(UART_ID, "Sat in main loop!\n");

        uint8_t eeprom_data[10];
        eeprom_read(i2c0, 0, eeprom_data, 10);

        printf("EEPROM Data: ");
        for (int i = 0; i < 10; i++)
        {
            printf("%02X ", eeprom_data[i]);
        }
        printf("\n");

        pca9554_set_outputs(i2c0, 0xFF);
        sleep_ms(500);
        pca9554_set_outputs(i2c0, 0x00);

        float temperature = read_max31725_temp(i2c0);
        printf("Temperature: %.2fC\n", temperature);

        sleep_ms(1000);
    }
}
