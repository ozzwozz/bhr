#include "I2CDevice.h"

I2CDevice::I2CDevice(i2c_inst_t *i2c, uint8_t address)
    : m_i2c(i2c)
    , m_address(address)
{
}

I2CDevice::~I2CDevice()
{
}

bool I2CDevice::read(uint8_t *data, size_t len)
{
    if (i2c_read_blocking(m_i2c, m_address, data, len, false) != PICO_ERROR_GENERIC)
    {
        return true;
    }
    return false;
}

bool I2CDevice::write(const uint8_t *data, size_t len)
{
    if (i2c_write_blocking(m_i2c, m_address, data, len, false) != PICO_ERROR_GENERIC)
    {
        return true;
    }
    return false;
}
