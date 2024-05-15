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

#include "PCA9554.h"
#include "MAX31725.h"
#include "M24M02.h"
#include "uart.h"
#include "DS1682.h"

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

static int chars_rxed = 0;

int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    // Put your timeout handler code in here
    return 0;
}


int main()
{
    stdio_init_all();

    // Timer code - This fires off the callback after 2000ms
    add_alarm_in_ms(2000, alarm_callback, NULL, false);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1'000'000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // gpio_init(PIN_CS_TEMP);
    // gpio_set_dir(PIN_CS_TEMP, GPIO_OUT);

    MAX31725 max31725(i2c0, MAX31725_ADDR);
    M24M02 m24m02(i2c0, EEPROM_ADDR);
    PCA9554 pca9554(i2c0, ATTENUATOR_1);

    UART uart(uart0, 9600, 1, 0);

    // Watchdog restart code
    if (watchdog_caused_reboot())
    {
        uart_puts(UART_ID, "Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }

    watchdog_enable(1 * 1'000'000, 1);

    char* to_send = "System Clock Frequency is %d Hz\n", clock_get_hz(clk_sys);
    uart_puts(UART_ID, to_send);

    // Configure onboard LED pin
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true)
    {
        gpio_put(LED_PIN, 1); // Turn LED on

        watchdog_update();

        float temperature = max31725.read_temperature();
        printf("Temperature: %.2fC\n", temperature);

        uint8_t eeprom_data[10];
        if (m24m02.read(0, eeprom_data, 10))
        {
            printf("Done");
        }

        printf("EEPROM Data: ");
        for (int i = 0; i < 10; i++)
        {
            printf("%02X ", eeprom_data[i]);
        }
        printf("\n");

        pca9554.set_outputs(0x00);
        sleep_ms(500);
        pca9554.set_outputs(0xFF);
        sleep_ms(500);

        if (uart.available())
        {
            char received_char;
            uart.read(&received_char, 1);
            printf("Recieved character from UART: %c\n", received_char);
        }
        gpio_put(LED_PIN, 0); // Turn LED off
    }

    return 0;
}