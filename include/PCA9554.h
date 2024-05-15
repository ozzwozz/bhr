#pragma once

#include "I2CDevice.h"

/**
 * @file PCA9554 Attenuator Driver
 * 
 */
class PCA9554 : public I2CDevice
{
    private:
        /* data */
    public:
        PCA9554(i2c_inst_t *i2c, uint8_t address);
        ~PCA9554();

        void set_outputs(uint8_t value);
        uint8_t read_inputs();
};
