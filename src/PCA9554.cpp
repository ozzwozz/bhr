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

bool PCA9554::set_outputs(const uint8_t value)
{
    uint8_t current_value = 0;
    I2CDevice::read(&current_value, 1);

    // Masking currently set values so that only explicit changes are made
    uint8_t new_value = (current_value & 0xFF) | (value & 0xFF);

    if(!I2CDevice::write(&new_value, 1))
    {
        return false;
    }

    return true;
}

bool PCA9554::read_inputs(uint8_t &value)
{
    if (!I2CDevice::read(&value, 1))
    {
        return false;
    }
    
    return true;
}

bool PCA9554::set_lna(const bool value)
{
    uint8_t current_value = 0;

    set_attenuator_enable(0);
    
    if (!I2CDevice::read(&current_value, 1))
    {
        return false;
    }

    // Mask to clear the 7th bit
    unsigned char clearMask = ~(1 << 6);

    // Mask to set the 7th bit
    unsigned char setMask = value << 6;

    // Clear the 7th bit
    current_value &= clearMask;

    // Set the 7th bit
    current_value |= setMask;

    if (!I2CDevice::write(&current_value, 1))
    {
        return false;
    }

    return true;
}

bool PCA9554::get_lna(bool &value)
{
    uint8_t current_value = 0;
    if (!I2CDevice::read(&current_value, 1))
    {
        return false;
    }

    // Mask to clear the 7th bit
    value = current_value << 6;

    return true;
}

bool PCA9554::set_attenuator_enable(bool value)
{
    uint8_t current_value = 0;

    set_lna(0);

    if (!I2CDevice::read(&current_value, 1))
    {
        return false;
    }

    // Mask to clear the 7th bit
    unsigned char clearMask = ~(1 << 7);

    // Mask to set the 7th bit
    unsigned char setMask = value << 7;

    // Clear the 7th bit
    current_value &= clearMask;

    // Set the 7th bit
    current_value |= setMask;

    if (!I2CDevice::write(&current_value, 1))
    {
        return false;
    }

    return true;
}

bool PCA9554::get_attenuator_enable(bool &value)
{
    uint8_t current_value = 0;
    if (!I2CDevice::read(&current_value, 1))
    {
        return false;
    }

    value = (current_value << 7);
    return true;
}

void PCA9554::set_power_state(bool value)
{
    gpio_put(m_power_enable_pin, value);
}

bool PCA9554::get_power_state(bool &value)
{
    return gpio_get(m_power_enable_pin);
}
