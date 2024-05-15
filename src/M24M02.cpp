#include "M24M02.h"

/**
 * @brief Construct a new M24M02::M24M02 object
 * 
 * @param i2c 
 * @param address 
 */
M24M02::M24M02(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

/**
 * @brief Destroy the M24M02::M24M02 object
 * 
 */
M24M02::~M24M02()
{
}

/**
 * @brief Read from the EEPROM device
 * 
 * @param address register address to access
 * @param data object to store the data in
 * @param len length of the data to be sent and received
 * @return true 
 * @return false 
 */
bool M24M02::read(uint32_t address, uint8_t *data, size_t len)
{
    bool success = true;
    uint8_t addr_buf[2] = {(address >> 8) & 0xFF, address & 0xFF};

    // Send EEPROM address
    if(!I2CDevice::write(addr_buf, len))
    {
        success = false;
    }

    if(!I2CDevice::read(data, len))
    {
        success = false;
    }

    return success;
}

/**
 * @brief Write to the EEPROM Device
 * 
 * @param address address to be written to 
 * @param data data to write to the device
 * @param len length of the data to be sent
 * @return true 
 * @return false 
 */
bool M24M02::write(uint32_t address, const uint8_t *data, size_t len)
{
    bool success = true;
    uint8_t addr_buf[3] = {(address >> 8) & 0xFF, address & 0xFF};

    // Send EEPROM address
    if(!I2CDevice::write(addr_buf, len))
    {
        success = false;
    }

    // Write Data
    if(!I2CDevice::write(data, len))
    {
        success = false;
    }

    return success;
}