#pragma once

#include "I2CDevice.h"

/**
 * @file MAX31725 - Local Temperature Sensor Driver
 * 
 */
class MAX31725 : public I2CDevice
{
    private:
        /* data */
    public:
        MAX31725(i2c_inst_t *i2c, uint8_t address);
        ~MAX31725();

        float read_temperature();
};
