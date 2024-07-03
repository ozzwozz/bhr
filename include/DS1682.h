#pragma once

#include "I2CDevice.h"
 
/// @class DS1682
/// @brief Driver for the DS1682 - Total-Elapsed-Time Recorder with Alarm
class DS1682 : public I2CDevice
{
public:
    /// @brief Construct a new DS1682 object
    /// @param i2c_inst i2c instance
    /// @param address device address
    DS1682(i2c_inst_t* i2c_inst, uint8_t address);

    /// @brief Destroy the DS1682 object
    ~DS1682();

    /// @brief
    void writeConfigRegister(uint8_t config);

    /// @brief Set time on DS1682
    /// @param timestamp 
    /// @return success is true
    bool setTime(const uint32_t timestamp);

    /// @brief Get the Time from DS1682
    /// @param timestamp get timestamp by reference
    /// @return success is true
    bool getTime(uint32_t &timestamp);

    /// @brief 
    /// @param counter 
    /// @return success is true
    bool getEventCounter(uint16_t &counter);

    /// @brief reset the ETC, EEPROM and event counter
    void reset();

private:
    /// @param etc_low_byte address for the lowest byte of the timestamp
    const uint8_t etc_low_byte = 0x05;
    /// @param etc_low_mid_byte address for the lower middle byte of the timestamp
    const uint8_t etc_low_mid_byte = 0x06;
    /// @param etc_high_mid_byte address for the upper middle byte of the timestamp
    const uint8_t etc_high_mid_byte = 0x07;
    /// @param etc_high_byte address for the upmost byte of the timestamp
    const uint8_t etc_high_byte = 0x08;

    /// @param event_counter_address_low address for the lower byte of the event counter
    const uint8_t event_counter_address_low = 0x09;
    /// @param event_counter_address_high address for the upper byte of the event counter
    const uint8_t event_counter_address_high = 0x0A;

    /// @param configuration_register address for the configuration register
    const uint8_t configuration_register = 0x00;

    union configuration_t
    {
        struct configuration_bits_t
        {
            bool event_counter_msb : 1;
            bool alarm_polarity : 1;
            bool reset_enable : 1;
            bool alarm_output_select : 1;
            bool write_memory_disabled_flag : 1;
            bool write_disabled_flag : 1;
            bool alarm_flag : 1;
            // bit 7 is unused
        } b;
        uint8_t i;
        
    } configuration;

    /// @param reset_command Command to reset the stored ETC values 
    uint8_t reset_command[2] {0x1D, 0x55};
};
