#include "DS1682.h"

/**
 * @brief Construct a new DS1682::DS1682 object
 * 
 * @param i2c_inst i2c instance
 * @param address device address
 */
DS1682::DS1682(i2c_inst_t* i2c_inst, uint8_t address) : I2CDevice(i2c_inst, address)
{
    // Constructor implementation if needed
}

/**
 * @brief Destroy the DS1682::DS1682 object
 */
DS1682::~DS1682()
{

}
/**
 * @brief Set time on DS1682
 * 
 * @param timestamp 
 * @return true 
 * @return false 
 */
bool DS1682::setTime(const uint32_t timestamp)
{
    uint8_t buffer[5]; // 5 bytes needed for timestamp data
    buffer[0] = 0x00; // Register address for setting time
    // Populate buffer with timestamp data (assumes little-endian byte order)
    buffer[1] = timestamp & 0xFF;
    buffer[2] = (timestamp >> 8) & 0xFF;
    buffer[3] = (timestamp >> 16) & 0xFF;
    buffer[4] = (timestamp >> 24) & 0xFF;
    
    return I2CDevice::write(buffer, sizeof(buffer));
}

/**
 * @brief Get the Time from DS1682
 * 
 * @return uint32_t current time
 */
uint32_t DS1682::getTime()
{
    uint32_t timestamp;
    uint8_t buffer[4]; // 4 bytes needed for timestamp data
    buffer[0] = 0x00; // Register address for reading time

    if (!I2CDevice::write(buffer, 1))
    {
        timestamp = 0; // Error in writing register address
    }

    if (!I2CDevice::read(buffer, sizeof(buffer)))
    {
        timestamp = 0; // Error in reading time data
    }

    // Convert the received data to a timestamp (assumes little-endian byte order)
    timestamp = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    return timestamp;
}

/**
 * @brief TODO
 * 
 * @return uint32_t 
 */
uint32_t DS1682::getUniqueID()
{
    uint32_t uniqueID;
    uint8_t buffer[8]; // 8 bytes needed for unique ID data
    buffer[0] = 0x08; // Register address for reading unique ID

    if (!I2CDevice::write(buffer, 1))
    {
        uniqueID = 0; // Error in writing register address
    }

    if (!I2CDevice::read(buffer, sizeof(buffer)))
    {
        uniqueID = 0; // Error in reading unique ID data
    }

    // Convert the received data to a unique ID (assumes little-endian byte order)
    uniqueID = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    return uniqueID;
}
