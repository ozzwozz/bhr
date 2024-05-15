#pragma once

#include "I2CDevice.h"

/**
 * @file M24M02 EEPROM Dvice Driver
 */
class M24M02 : public I2CDevice
{
    public:
        M24M02(i2c_inst_t *i2c, uint8_t address);
        ~M24M02();

        bool read(uint32_t address, uint8_t *data, size_t len);
        bool write(uint32_t address, const uint8_t *data, size_t len);
};
