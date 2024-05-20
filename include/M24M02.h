#pragma once

#include "I2CDevice.h"


/// @class M24M02 EEPROM Device Driver
/// @brief Driver for the M24M02
class M24M02 : public I2CDevice
{
    public:
        /// @brief Construct a new M24M02 object
        /// @param i2c 
        /// @param address 
        M24M02(i2c_inst_t *i2c, uint8_t address);

        /// @brief Destroy the M24M02 object
        ~M24M02();

        /// @brief Read from the EEPROM device
        /// @param address register address to access
        /// @param data object to store the data in
        /// @param len length of the data to be sent and received
        /// @return success is true
        bool read(const uint32_t address, uint8_t *data, size_t len);
        
        /// @brief Write to the EEPROM Device
        /// @param address address to be written to 
        /// @param data data to write to the device
        /// @param len length of the data to be sent
        /// @return success is true 
        bool write(const uint32_t address, const uint8_t *data, size_t len);
};
