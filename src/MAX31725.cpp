#include "MAX31725.h"

MAX31725::MAX31725(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
    // Initialize External interrupt trigger
    gpio_init(m_overtemp_pin);
    gpio_set_dir(m_overtemp_pin, GPIO_IN);
    gpio_pull_down(m_overtemp_pin); // Set External interrupt trigger as input with pull-down resistor

    set_over_temp_limit();
    
    // Set up interrupt for GPIO 15
    irq_set_exclusive_handler(15, (irq_handler_t)over_temp_irq_handler);

}

MAX31725::~MAX31725()
{
}

bool MAX31725::set_over_temp_limit()
{
    uint8_t tx_data[2] {cmd_set_temp_limit, m_temperature_limit_C};

    if (!write(tx_data, 1))
    {
        return false;
    }

    return true;
}

bool MAX31725::read_temperature(float &temperature)
{
    uint8_t rx_data[2];

    if (!write(&cmd_read_temperature, 1))
    {
        return false;
    }

    if (!read(rx_data, 2))
    {
        return false;
    }

    int16_t temp_data = (rx_data[0] << 8) | rx_data[1];
    temperature = (temp_data / 256.0);

    return true;
}

void MAX31725::over_temp_irq_handler(void *context)
{
    // TODO: Issue #2 Implement flag set or add message to the UART Tx buffer
}
