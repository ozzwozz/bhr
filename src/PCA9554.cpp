#include "PCA9554.h"

PCA9554::PCA9554(i2c_inst_t *i2c, uint8_t address, uint power_enable_pin) : I2CDevice(i2c, address), m_power_enable_pin(power_enable_pin)
{
    gpio_init(m_power_enable_pin);
    gpio_set_dir(m_power_enable_pin, GPIO_OUT);
    
    set_power_state(1);
}

PCA9554::~PCA9554()
{
    set_power_state(0);
}

void PCA9554::configuration()
{
    // All pins as outputs
    uint8_t config = 0x00;
    uint8_t command[2] {config_register, config}; 
    int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
}

bool PCA9554::set_outputs(const uint8_t value)
{
    uint8_t current_value = 0;

    if (read_inputs(current_value))
    {
        // Masking currently set values so that only explicit changes are made
        uint8_t current_value_shifted = (current_value >> 6);
        uint8_t new_value = (current_value_shifted << 6) | (value & 0xFF);

        uint8_t command[2] {output_port_register, current_value};

        int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
        if (ret == PICO_ERROR_GENERIC)
        {
            return false;
        }
    }

    return true;
}

bool PCA9554::read_inputs(uint8_t &value)
{
    if (!I2CDevice::write(&output_port_register, 1))
    {
        return false;
    }

    if (!I2CDevice::read(&value, 1))
    {
        return false;
    }
    
    return true;
}

bool PCA9554::set_lna(const bool value)
{
    uint8_t current_value = 0;

    if (read_inputs(current_value))
    {
        uint8_t new_value = current_value ^ (value << 6);

        uint8_t command[2] {output_port_register, new_value};

        int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
        if (ret == PICO_ERROR_GENERIC)
        {
            return false;
        }
    }

    return true;
}

bool PCA9554::get_lna(bool &value)
{
    uint8_t current_value = 0;

    read_inputs(current_value);

    // Mask to clear the 7th bit
    value = current_value >> 6;

    return true;
}

bool PCA9554::set_attenuator_enable(bool value)
{

    uint8_t current_value = 0;

    read_inputs(current_value);

    uint8_t new_value = current_value ^ (value << 7); 
    uint8_t command[2] {output_port_register, new_value};

    int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    return true;
}

bool PCA9554::get_attenuator_enable(bool &value)
{
    uint8_t current_value = 0;

    read_inputs(current_value);

    value = (current_value >> 7);
    return true;
}

void PCA9554::set_power_state(bool value)
{
    gpio_put(m_power_enable_pin, value);
 
    if (value == 1)
    {
        sleep_us(10);
        configuration();
        set_attenuator_enable(1);
        set_outputs(0x3C);
    }
}

bool PCA9554::get_power_state(bool &value)
{
    return gpio_get(m_power_enable_pin);
}
