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
#include "USB_Handler.h"
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

#define DS1682_ADDR 0x6B
#define EEPROM_ADDR 0x50

#define CMD_READ_TEMP 0x00
#define MAX31725_ADDR 0x48

// I2C defines
#define I2C0_PORT i2c0
#define I2C0_SDA 4
#define I2C0_SCL 5

#define I2C1_PORT i2c1
#define I2C1_SDA 2
#define I2C1_SCL 3

#define ATTENUATOR_1 0X20
#define ATTENUATOR_2 0X21
#define ATTENUATOR_3 0X22
#define ATTENUATOR_4 0X23
#define ATTENUATOR_5 0X24

#define ATTENUATOR_1_POWER 7
#define ATTENUATOR_2_POWER 8
#define ATTENUATOR_3_POWER 9
#define ATTENUATOR_4_POWER 10
#define ATTENUATOR_5_POWER 11

#define ATTENUATOR_REG_OUT 0x01

int main()
{
    stdio_init_all();

    // Watchdog restart code
    if (watchdog_caused_reboot())
    {
        printf("Rebooted by Watchdog!\n");
        // Whatever action you may take if a watchdog caused a reboot
    }

    // 0x7fffff is roughly 8.3 seconds and the maximum
    watchdog_enable(0x7fffff, 1);

    // Configure onboard LED pin
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1); // Turn LED on
    
    // I2C Initialisation. Using it at 100Khz.
    i2c_init(I2C0_PORT, 100'000);
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA);
    gpio_pull_up(I2C0_SCL);

    i2c_init(I2C1_PORT, 100'000);
    gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SDA);
    gpio_pull_up(I2C1_SCL);

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1'000'000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    sleep_ms(2000);

    MAX31725 max31725 = MAX31725(i2c0, MAX31725_ADDR);
    M24M02 m24m02 = M24M02(i2c0, EEPROM_ADDR);
    SI53361 si53361 = SI53361();
    DS1682 ds1682 = DS1682(i2c0, DS1682_ADDR);

    ADC adc = ADC();

    PCA9554 pca9554_1 = PCA9554(i2c1, ATTENUATOR_1, ATTENUATOR_1_POWER);
    PCA9554 pca9554_2 = PCA9554(i2c1, ATTENUATOR_2, ATTENUATOR_2_POWER);
    PCA9554 pca9554_3 = PCA9554(i2c1, ATTENUATOR_3, ATTENUATOR_3_POWER);
    PCA9554 pca9554_4 = PCA9554(i2c1, ATTENUATOR_4, ATTENUATOR_4_POWER);
    PCA9554 pca9554_5 = PCA9554(i2c1, ATTENUATOR_5, ATTENUATOR_5_POWER);

    USB_Handler usb_handler = USB_Handler(max31725, m24m02, si53361, pca9554_1,
                    pca9554_2, pca9554_3, pca9554_4, pca9554_5, adc, ds1682);
    
    // Turn off LED outside of main loop
    gpio_put(LED_PIN, 0); // Turn LED off
    sleep_ms(50);

    while (true)
    {
        // LED Flashing while developing so I know the board doesn't hang
        sleep_ms(50);

        watchdog_update();

        uint8_t input[5] = {0, 0, 0, 0, 0};

        for(int i = 0; i < sizeof(input); i++)
        {   
            input[i] = getchar_timeout_us(10);
        }

        // if command byte is populated
        if (input[0] != PICO_ERROR_TIMEOUT)
        {
            usb_handler.decode_message(input);
        }

        sleep_ms(50);
    }

    return 0;
}
