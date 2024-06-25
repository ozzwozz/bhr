#include "DS1682.h"

DS1682::DS1682(i2c_inst_t* i2c_inst, uint8_t address) : I2CDevice(i2c_inst, address)
{
    configuration.i = 0xFF;
    configuration.b.alarm_polarity = 0;
    configuration.b.write_disabled_flag = 0;
    configuration.b.write_memory_disabled_flag = 0;
    
    // write configuration twice to set
    write(&configuration.i, 1);
    write(&configuration.i, 1);

    // write alarm
    // write user memory

    // write-protect alarm

    // write-protect ETC
    // write-protect Event counter
}

DS1682::~DS1682()
{

}

void DS1682::reset()
{
    configuration.b.write_disabled_flag = 0;
    configuration.b.reset_enable = 1;
    write(reset_command, 2);
    write(reset_command, 2);
    configuration.b.write_disabled_flag = 1;
    configuration.b.reset_enable = 0;
    write(reset_command, 2);
    write(reset_command, 2);
}

void DS1682::writeConfigRegister(uint8_t config)
{
    uint8_t command {0x07 | config};
    write(&command, 1);
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
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, sizeof(buffer), true);
    if (ret == PICO_ERROR_GENERIC)
    {
        timestamp = 0; // Error in writing register address
        return false;
    }

    // receive time data
    if (I2CDevice::read(buffer, 4) == PICO_ERROR_GENERIC)
    {
        timestamp = 0; // Error in reading time data
        return false;
    }

    // Convert the received data to a timestamp (assumes little-endian byte order)
    timestamp = ( buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];

    return true;
}

bool DS1682::getEventCounter(uint16_t &counter)
{
    uint8_t buffer[2]; // 8 bytes needed for unique ID data

    // Register address for reading the event counter
    buffer[0] = event_counter_address_low;
    buffer[1] = event_counter_address_high;

    // request time data
    // using a special i2c_write because we want to keep master control
    int ret = i2c_write_blocking(m_i2c, m_address, buffer, sizeof(buffer), true);
    if (ret == PICO_ERROR_GENERIC)
    {
        counter = 0; // Error in writing register address
        return false;
    }

    if (I2CDevice::read(buffer, sizeof(buffer)) == PICO_ERROR_GENERIC)
    {
        counter = 0; // Error in reading unique ID data
        return false;
    }

    // Convert the received data to a unique ID (assumes little-endian byte order)
    counter = (buffer[1] << 8) + buffer[0];
    return true; 
}
