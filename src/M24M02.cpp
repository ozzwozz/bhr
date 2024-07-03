#include "M24M02.h"

M24M02::M24M02(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

M24M02::~M24M02()
{
}

bool M24M02::read(const uint32_t address, uint8_t *data, size_t len)
{
    uint8_t addr_buf[2] {(address >> 8) & 0xFF, address & 0xFF};

    // Send EEPROM address
    if (!I2CDevice::write(addr_buf, len))
    {
        return false;
    }

    // read returned data
    if (!I2CDevice::read(data, len))
    {
        return false;
    }

    return true;
}

bool M24M02::write(const uint32_t address, const uint8_t *data, size_t len)
{
    uint8_t addr_buf[3] {(address >> 8) & 0xFF, address & 0xFF};

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

bool M24M02::get_software_id(uint32_t &software_id)
{
    uint8_t buffer[4]; // 4 bytes needed for unique ID data

    // Register address for reading unique ID
    buffer[0] = (software_id_address_low >> 8);
    buffer[1] = software_id_address_low & 0xFF;

    // request time data
    // using a special i2c_write because we want to keep master control
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, 2, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        software_id = 0; // Error in writing register address
        return false;
    }

    ret = i2c_read_blocking(m_i2c, m_address, buffer, 4, false);
    if (ret == PICO_ERROR_GENERIC)
    {
        software_id = 0; // Error in reading unique ID data
        return false;
    }

    // Convert the received data to a unique ID (assumes little-endian byte order)
    software_id = ( buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3] & 0xFF);
    return true;
}


bool M24M02::get_hardware_id(uint32_t &hardware_id)
{
    uint8_t buffer[4]; // 4 bytes needed for unique ID data

    // Register address for reading unique ID
    buffer[0] = (hardware_id_address_low >> 8);
    buffer[1] = hardware_id_address_low & 0xFF;

    // request time data
    // using a special i2c_write because we want to keep master control
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, 2, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        hardware_id = 0; // Error in writing register address
        return false;
    }

    ret = i2c_read_blocking(m_i2c, m_address, buffer, 4, false);
    if (ret == PICO_ERROR_GENERIC)
    {
        hardware_id = 0; // Error in reading unique ID data
        return false;
    }

    // Convert the received data to a unique ID (assumes little-endian byte order)
    hardware_id = ( buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3] & 0xFF);
    return true;
}

bool M24M02::set_software_id(uint32_t &software_id)
{
    uint8_t buffer[6]; // 6 bytes needed for unique ID data

    // Register address for reading unique ID
    buffer[0] = (software_id_address_low >> 8);
    buffer[1] = software_id_address_low & 0xFF;

    buffer[2] = (software_id >> 24);
    buffer[3] = (software_id >> 16);
    buffer[4] = (software_id >> 8);
    buffer[5] = software_id & 0xFF;
    
    // using a special i2c_write because we want to keep master control
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, sizeof(buffer), false);
    if (ret == PICO_ERROR_GENERIC)
    {
        software_id = 0; // Error in writing register address
        return false;
    }

    return true;
}

bool M24M02::set_hardware_id(uint32_t &hardware_id)
{
    uint8_t buffer[6]; // 6 bytes needed for unique ID data

    // Register address for reading unique ID
    buffer[0] = (hardware_id_address_low >> 8);
    buffer[1] = hardware_id_address_low & 0xFF;

    buffer[2] = (hardware_id >> 24);
    buffer[3] = (hardware_id >> 16);
    buffer[4] = (hardware_id >> 8);
    buffer[5] = hardware_id & 0xFF;
    
    // using a special i2c_write because we want to keep master control
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, sizeof(buffer), false);
    if (ret == PICO_ERROR_GENERIC)
    {
        hardware_id = 0; // Error in writing register address
        return false;
    }

    return true;
}
