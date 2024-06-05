#pragma once

#include "pico/stdlib.h"
#include "hardware/i2c.h"


/// @class I2CDevice
/// @brief I2C super class for I2C Device driver to inherit from
class I2CDevice
{
public:
    /// @brief Construct a new I2CDevice object
    /// @param i2c i2c instance
    /// @param address target device address
    I2CDevice(i2c_inst_t *i2c, uint8_t address);

    /// @brief Destroy the I2CDevice object
    virtual ~I2CDevice();

    /// @brief Read data sent from the target device
    /// @param data pointer to address of data to be sent
    /// @param len length of the data to be sent
    /// @return success is true
    bool read(uint8_t *data, const size_t len);

    /// @brief Write given data to the target device
    /// @param data pointer to address of data to be sent
    /// @param len length of the data to be sent
    /// @return success is true
    bool write(const uint8_t *data, const size_t len);
    
protected:
    /// @param m_i2c member i2c instance 
    i2c_inst_t *m_i2c;
    /// @param m_address member i2c device address 
    uint8_t m_address;
};
