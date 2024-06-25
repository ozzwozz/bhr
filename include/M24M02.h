#pragma once

#include "I2CDevice.h"

/// @class M24M02
/// @brief Driver for the M24M02 - EEPROM Device Driver
class M24M02 : public I2CDevice
{
public:
    /// @brief Construct a new M24M02 object
    /// @param i2c i2c instance
    /// @param address EEPROM device addres
    M24M02(i2c_inst_t *i2c, uint8_t address);

    /// @brief Destroy the M24M02 object
    ~M24M02();

    /// @brief Read from the EEPROM device
    /// @param address register address to access
    /// @param data object to store the data in
    /// @param len length of  the data to be sent and received
    /// @return success is true
    bool read(const uint32_t address, uint8_t *data, const size_t len);
    
    /// @brief Write to the EEPROM Device
    /// @param address address to be written to 
    /// @param data data to write to the device
    /// @param len length of the data to be sent
    /// @return success is true 
    bool write(const uint32_t address, const uint8_t *data, const size_t len);
    
    /// @brief Get the software ID
    /// @param software_id 
    /// @return 
    bool get_software_id(uint32_t &software_id);

    /// @brief Get the hardware ID
    /// @param hardware_id 
    /// @return 
    bool get_hardware_id(uint32_t &hardware_id);

    /// @brief Set the software ID
    /// @param software_id 
    /// @return 
    bool set_software_id(uint32_t &software_id);

    /// @brief Set the hardware ID
    /// @param hardware_id 
    /// @return 
    bool set_hardware_id(uint32_t &hardware_id);

private:
    /// @param software_id_address_low address for the lowest byte of the ID
    const uint8_t software_id_address_low = 0x0000;
    /// @param software_id_address_low_mid address for the lower middle byte of the ID
    const uint8_t software_id_address_low_mid = 0x0001;
    /// @param software_id_address_high_mid address for the upper middle byte of the ID
    const uint8_t software_id_address_high_mid = 0x0002;
    /// @param software_id_address_high address for the upmost byte of the ID
    const uint8_t software_id_address_high = 0x0003;

    /// @param hardware_id_address_low address for the lowest byte of the ID
    const uint8_t hardware_id_address_low = 0x0004;
    /// @param hardware_id_address_low_mid address for the lower middle byte of the ID
    const uint8_t hardware_id_address_low_mid = 0x0005;
    /// @param hardware_id_address_high_mid address for the upper middle byte of the ID
    const uint8_t hardware_id_address_high_mid = 0x0006;
    /// @param hardware_id_address_high address for the upmost byte of the ID
    const uint8_t hardware_id_address_high = 0x0007;
};
