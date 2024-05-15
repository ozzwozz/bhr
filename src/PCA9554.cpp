#include "PCA9554.h"

PCA9554::PCA9554(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

PCA9554::~PCA9554()
{
}

void PCA9554::set_outputs(const uint8_t value)
{
    uint8_t current_value = 0;
    I2CDevice::read(&current_value, 1);

    // Masking currently set values so that only explicit changes are made
    uint8_t new_value = (current_value & 0xFF) | (value & 0xFF);

    if(!I2CDevice::write(&new_value, 1))
    {
        return;
    }
}

uint8_t PCA9554::read_inputs()
{
    uint8_t data;
    I2CDevice::read(&data, 1);
    return data;
}
