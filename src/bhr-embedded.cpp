#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "PCA9554.h"
#include "MAX31725.h"
#include "M24M02.h"
#include "uart.h"
#include "DS1682.h"
#include "adc.h"
#include "SI53361.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS_TEMP 17
#define PIN_SCK  18
#define PIN_MOSI 19

#define DS1682_ADDR 0xD6
#define EEPROM_ADDR 0x50

#define CMD_READ_TEMP 0x00
#define MAX31725_ADDR 0x48

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C0_PORT i2c0
#define I2C0_SDA 8
#define I2C0_SCL 9

#define I2C1_PORT i2c1
#define I2C1_SDA 14
#define I2C1_SCL 15

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

int main()
{
    stdio_init_all();

    // Configure onboard LED pin
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C0_PORT, 400*1000);
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA);
    gpio_pull_up(I2C0_SCL);

    i2c_init(I2C1_PORT, 400*1000);
    gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SDA);
    gpio_pull_up(I2C1_SCL);

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1'000'000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    MAX31725 max31725 = MAX31725(i2c0, MAX31725_ADDR);
    M24M02 m24m02 = M24M02(i2c0, EEPROM_ADDR);
    SI53361 si53361 = SI53361();

    PCA9554 pca9554_1 = PCA9554(i2c1, ATTENUATOR_1);
    PCA9554 pca9554_2 = PCA9554(i2c1, ATTENUATOR_2);
    PCA9554 pca9554_3 = PCA9554(i2c1, ATTENUATOR_3);
    PCA9554 pca9554_4 = PCA9554(i2c1, ATTENUATOR_4);
    PCA9554 pca9554_5 = PCA9554(i2c1, ATTENUATOR_5);
    DS1682 ds1682 = DS1682(i2c0, DS1682_ADDR);

    ADC adc = ADC();
    UART uart = UART(uart0, 9600, 1, 0, max31725, m24m02, si53361, pca9554_1,
                    pca9554_2, pca9554_3, pca9554_4, pca9554_5, adc, ds1682);

    // Watchdog restart code
    if (watchdog_caused_reboot())
    {
        uart_puts(UART_ID, "Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }

    watchdog_enable(1 * 1'000'000, 1);

    gpio_put(LED_PIN, 0); // Turn LED off

    while (true)
    {
        gpio_put(LED_PIN, 1); // Turn LED on

        watchdog_update();

        if (uart.available())
        {
            char received_char;
            uart.read(&received_char, 1);
            printf("Received character from UART: %c\n", received_char);
        }
    }

    return 0;
}
