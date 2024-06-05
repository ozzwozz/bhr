#include "DS1682.h"

DS1682::DS1682(i2c_inst_t* i2c_inst, uint8_t address) : I2CDevice(i2c_inst, address)
{
    // write configuration
    // write alarm
    // write user memory

    // write-protect alarm
    // write-protect ETC
    // write-protect Event counter
    setTime(0x00);
}

DS1682::~DS1682()
{

}

bool DS1682::setTime(const uint32_t timestamp)
{
    uint8_t buffer[5]; // 1 byte is register address + 4 for data
    buffer[0] = 0x00; // Register address for setting time
    
    // Populate buffer with timestamp data (assumes little-endian byte order)
    buffer[4] = timestamp & 0xFF;
    buffer[3] = (timestamp >> 8) & 0xFF;
    buffer[2] = (timestamp >> 16) & 0xFF;
    buffer[1] = (timestamp >> 24) & 0xFF;
    
    return I2CDevice::write(buffer, sizeof(buffer));
}

bool DS1682::getTime(uint32_t &timestamp)
{
    uint8_t buffer[7]; // 4 bytes needed for timestamp data
    buffer[0] = etc_low_byte; // Register address for reading time
    buffer[1] = etc_low_mid_byte;
    buffer[2] = etc_high_mid_byte;
    buffer[3] = etc_high_byte;

    // request time data
    // using a special i2c_write because we want to keep master control
    // int ret = i2c_write_blocking(m_i2c, m_address, NULL, 0, true);
    // if (ret == PICO_ERROR_GENERIC)
    // {
    //     timestamp = 0; // Error in writing register address
    //     return false;
    // }

    // receive time data
    if (I2CDevice::read(buffer, 7) == PICO_ERROR_GENERIC)
    {
        timestamp = 0; // Error in reading time data
        return false;
    }

    // Convert the received data to a timestamp (assumes little-endian byte order)
    timestamp = ( buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];

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

    if (I2CDevice::write(buffer, 1) == PICO_ERROR_GENERIC)
    {
        device_id = 0; // Error in writing register address
        return false;
    }

    if (I2CDevice::read(buffer, sizeof(buffer)) == PICO_ERROR_GENERIC)
    {
        device_id = 0; // Error in reading unique ID data
        return false;
    }

    // Convert the received data to a unique ID (assumes little-endian byte order)
    device_id = ( buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
    return true;
}
