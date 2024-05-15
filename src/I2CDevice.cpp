#include "I2CDevice.h"


/// @brief Construct a new I2CDevice::I2CDevice object
/// @param i2c i2c instance
/// @param address target device address
I2CDevice::I2CDevice(i2c_inst_t *i2c, uint8_t address)
    : m_i2c(i2c)
    , m_address(address)
{
}


/// @brief Destroy the I2CDevice::I2CDevice object
I2CDevice::~I2CDevice()
{
}

/// @brief Read datasent from the target device
/// @param data pointer to address of data to be sent
/// @param len length of the data to be sent
/// @return true 
/// @return false 
bool I2CDevice::read(uint8_t *data, size_t len)
{
    bool success = false;
    if (i2c_read_blocking(m_i2c, m_address, data, len, false) != PICO_ERROR_GENERIC)
    {
        success = true;
    }
    return success;
}


/// @brief Write given data to the target device
/// @param data pointer to address of data to be sent
/// @param len length of the data to be sent
/// @return true 
/// @return false 
bool I2CDevice::write(const uint8_t *data, size_t len)
{
    bool success = false;
    if (i2c_write_blocking(m_i2c, m_address, data, len, false) != PICO_ERROR_GENERIC)
    {
        success = true;
    }
    return success;
}
