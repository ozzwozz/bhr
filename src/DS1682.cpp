#include "DS1682.h"

DS1682::DS1682(i2c_inst_t* i2c_inst, uint8_t address) : I2CDevice(i2c_inst, address)
{
    // Constructor implementation if needed
}

DS1682::~DS1682()
{

}

bool DS1682::setTime(const uint32_t timestamp)
{
    uint8_t buffer[5]; // 1 byte is register address + 4 for data
    buffer[0] = 0x00; // Register address for setting time
    
    // Populate buffer with timestamp data (assumes little-endian byte order)
    buffer[1] = timestamp & 0xFF;
    buffer[2] = (timestamp >> 8) & 0xFF;
    buffer[3] = (timestamp >> 16) & 0xFF;
    buffer[4] = (timestamp >> 24) & 0xFF;
    
    return I2CDevice::write(buffer, sizeof(buffer));
}

bool DS1682::getTime(uint32_t &timestamp)
{
    uint8_t buffer[4]; // 4 bytes needed for timestamp data
    buffer[0] = etc_low_byte; // Register address for reading time
    buffer[1] = etc_low_mid_byte;
    buffer[2] = etc_high_mid_byte;
    buffer[3] = etc_high_byte;

    // request time data
    if (!I2CDevice::write(buffer, 4))
    {
        timestamp = 0; // Error in writing register address
        return false;
    }

    // receive time data
    if (!I2CDevice::read(buffer, 4))
    {
        timestamp = 0; // Error in reading time data
        return false;
    }

    // Convert the received data to a timestamp (assumes little-endian byte order)
    timestamp = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
    return true;
}

bool DS1682::getUniqueID(uint32_t &device_id)
{
    uint8_t buffer[8]; // 8 bytes needed for unique ID data

    // Register address for reading unique ID
    buffer[0] = unique_id_address_low;
    buffer[1] = unique_id_address_low_mid;
    buffer[2] = unique_id_address_high_mid;
    buffer[3] = unique_id_address_high;

    if (!I2CDevice::write(buffer, 1))
    {
        device_id = 0; // Error in writing register address
        return false;
    }

    if (!I2CDevice::read(buffer, sizeof(buffer)))
    {
        device_id = 0; // Error in reading unique ID data
        return false;
    }

    // Convert the received data to a unique ID (assumes little-endian byte order)
    device_id = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
    return true;
}
