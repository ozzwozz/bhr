#include "PCA9554.h"

PCA9554::PCA9554(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

PCA9554::~PCA9554()
{
}

void PCA9554::set_outputs(uint8_t value)
{
    uint8_t current_value = 0;
    I2CDevice::read(&current_value, 1);

    uint8_t new_value = (current_value & 0xFF) | (value & 0xFF);

    // TODO: probably not 16
    if(!I2CDevice::write(&new_value, 1))
    {
        ; // Pass because I don't know what I wanted here
    }
}

uint8_t PCA9554::read_inputs()
{
    uint8_t data;
    I2CDevice::read(&data, 1);
    return data;
}