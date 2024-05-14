#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"

class I2CDevice
{
private:
    i2c_inst_t *m_i2c;
    uint8_t m_address;
public:
    I2CDevice(i2c_inst_t *i2c, uint8_t address);
    ~I2CDevice();
    bool read(uint8_t *data, size_t len);
    bool write(const uint8_t *data, size_t len);
};
