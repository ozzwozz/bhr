#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"

/**
 * @file I2C super class for I2C Device driver to inherit from 
 * 
 */
class I2CDevice
{
private:
    /**
     * @param m_i2c member i2c instance
     */
    i2c_inst_t *m_i2c;
    /**
     * @param m_address member i2c device address
     */
    uint8_t m_address;
public:
    I2CDevice(i2c_inst_t *i2c, uint8_t address);
    ~I2CDevice();
    bool read(uint8_t *data, size_t len);
    bool write(const uint8_t *data, size_t len);
};
