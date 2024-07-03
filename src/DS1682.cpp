#include "DS1682.h"

DS1682::DS1682(i2c_inst_t* i2c_inst, uint8_t address) : I2CDevice(i2c_inst, address)
{
    configuration.b.alarm_flag = 0;
    configuration.b.alarm_polarity = 0;
    configuration.b.write_disabled_flag = 0;
    configuration.b.write_memory_disabled_flag = 0;
    configuration.i = 0;

    uint8_t config_command[2] = {configuration_register, configuration.i};
    // write configuration to set
    write(config_command, 2);
}

DS1682::~DS1682()
{

}

void DS1682::reset()
{
    configuration.b.reset_enable = 1;
    uint8_t config_command[2] {0x07 , configuration.i};
    write(config_command, 2);
    write(reset_command, 2);
    write(reset_command, 2);
    configuration.b.reset_enable = 0;
    config_command[2] = configuration.i;
    write(config_command, 2);
    write(reset_command, 2);
    write(reset_command, 2);
}

void DS1682::writeConfigRegister(uint8_t config)
{
    uint8_t command[2] {0x07 , config};
    write(command, 2);
}

bool DS1682::getTime(uint32_t &timestamp)
{
    uint8_t register_address[4]; // 4 bytes needed for timestamp addressing
    uint8_t buffer[4]; // 4 bytes needed for timestamp data

    register_address[0] = etc_low_byte; // Register address for reading time

    int ret = i2c_write_blocking(m_i2c, m_address, register_address, 1, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        timestamp = 0; // Error in writing register address
        return false;
    }

    // receive time data
    ret = i2c_read_blocking(m_i2c, m_address, buffer, 4, false);
    if (ret == PICO_ERROR_GENERIC)
    {
        timestamp = 0; // Error in reading time data
        return false;
    }

    // Convert the received data to a timestamp (assumes little-endian byte order)
    timestamp = ( buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | (buffer[0] & 0xFF);

    return true;
}

bool DS1682::getEventCounter(uint16_t &counter)
{
    uint8_t register_address[2]; // 8 bytes needed for unique ID data

    // Register address for reading the event c ounter
    register_address[0] = event_counter_address_low;

    // request time data
    // using a special i2c_write because we want to keep master control
    int ret = i2c_write_blocking(m_i2c, m_address, register_address, 1, false);
    if (ret == PICO_ERROR_GENERIC)
    {
        counter = 0; // Error in reading time data
        return false;
    }
    uint8_t buffer[2]; // 2 bytes needed for event data

    // receive time data
    ret = i2c_read_blocking(m_i2c, m_address, buffer, 2, false);
    if (ret == PICO_ERROR_GENERIC)
    {
        counter = 0; // Error in reading time data
        return false;
    }

    // Convert the received data to 16 bit counter (assumes big-endian byte order)
    counter = (buffer[1] << 8) | (buffer[0] & 0xFF);
    return true; 
}
