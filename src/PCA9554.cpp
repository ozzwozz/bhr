#include "PCA9554.h"

PCA9554::PCA9554(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

PCA9554::~PCA9554()
{
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

uint8_t PCA9554::read_inputs()
{
    uint8_t data;
    I2CDevice::read(&data, 1);
    return data;
}

bool PCA9554::set_lna(const bool value)
{
    uint8_t current_value = 0;
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
