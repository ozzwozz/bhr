#include "M24M02.h"

M24M02::M24M02(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

M24M02::~M24M02()
{
}

bool M24M02::read(const uint32_t address, uint8_t *data, size_t len)
{
    uint8_t addr_buf[2] = {(address >> 8) & 0xFF, address & 0xFF};

    // Send EEPROM address
    if (!I2CDevice::write(addr_buf, len))
    {
        return false;
    }

    if (!I2CDevice::read(data, len))
    {
        return false;
    }

    return true;
}

bool M24M02::write(const uint32_t address, const uint8_t *data, size_t len)
{
    uint8_t addr_buf[3] = {(address >> 8) & 0xFF, address & 0xFF};

    // Send EEPROM address
    if (!I2CDevice::write(addr_buf, len))
    {
        return false;
    }

    // Write Data
    if (!I2CDevice::write(data, len))
    {
        return false;
    }

    return true;
}
