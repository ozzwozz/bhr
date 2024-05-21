#pragma once

#include "I2CDevice.h"
 
/// @class DS1682 - Total-Elapsed-Time Recorder with Alarm
/// @brief Driver for the DS1682
class DS1682 : public I2CDevice {
    private:
    const uint8_t etc_low_byte = 0x05;
    const uint8_t etc_low_mid_byte = 0x06;
    const uint8_t etc_high_mid_byte = 0x07;
    const uint8_t etc_high_byte = 0x08;

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
};
