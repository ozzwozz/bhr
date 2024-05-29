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

        /// @brief Set time on DS1682
        /// @param timestamp 
        /// @return success is true
        bool setTime(const uint32_t timestamp);

        /// @brief Get the Time from DS1682
        /// @param timestamp get timestamp by reference
        /// @return success is true
        bool getTime(uint32_t &timestamp);

        /// @brief Get the device ID
        /// @param device_id get device id by reference
        /// @return success is true
        bool getUniqueID(uint32_t &device_id);

    private:
        /// @param etc_low_byte address for the lowest byte of the timestamp
        const uint8_t etc_low_byte = 0x05;
        /// @param etc_low_mid_byte address for the lower middle byte of the timestamp
        const uint8_t etc_low_mid_byte = 0x06;
        /// @param etc_high_mid_byte address for the upper middle byte of the timestamp
        const uint8_t etc_high_mid_byte = 0x07;
        /// @param etc_high_byte address for the upmost byte of the timestamp
        const uint8_t etc_high_byte = 0x08;

        /// @param unique_id_address_low address for the lowest byte of the ID
        const uint8_t unique_id_address_low = 0x0B;
        /// @param unique_id_address_low_mid address for the lower middle byte of the ID
        const uint8_t unique_id_address_low_mid = 0x0C;
        /// @param unique_id_address_high_mid address for the upper middle byte of the ID
        const uint8_t unique_id_address_high_mid = 0x0D;
        /// @param unique_id_address_high address for the upmost byte of the ID
        const uint8_t unique_id_address_high = 0x0E;
};
