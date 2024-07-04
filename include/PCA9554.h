#pragma once

#include "I2CDevice.h"


/// @class PCA9554 Attenuator Driver
/// @brief Driver for the PCA9554
class PCA9554 : public I2CDevice
{
private:
    const uint8_t output_port_register = 0x01;
    const uint8_t config_register = 0x03;

    void configuration();
    
protected:
    /// @param m_power_enable_pin Pin to control power to the attenuator card
    uint m_power_enable_pin;

public:
    /// @brief Construct a new PCA9554 object
    /// @param i2c i2c instance
    /// @param address address of the target
    /// @param power_enable_pin
    PCA9554(i2c_inst_t *i2c, uint8_t address, uint power_enable_pin);

    /// @brief Destroy the PCA9554 object
    ~PCA9554();

    /// @brief Write data to the attenuator cards
    /// @param value Value to set the attenuator output to
    /// @return success is true
    bool set_outputs(const uint8_t value);

    /// @brief Read current settings from the attenuator cards
    /// @param uint8_t current output settings
    /// @return success of the read is true
    bool read_inputs(uint8_t &value);

    /// @brief Write data to the attenuator cards
    /// @param value value to set the LNA bit to
    /// @return success is true
    bool set_lna(bool value);
    
    /// @brief Get the LNA status from the attenuator card
    /// @param value get the LNA status by reference
    /// @return success is true
    bool get_lna(bool &value);

    /// @brief Write data to the attenuator cards
    /// @param value set the attenuator enable pin
    /// @return success is true
    bool set_attenuator_enable(bool value);
    
    /// @brief Write data to the attenuator cards
    /// @param value get the attenuator enable status by reference
    /// @return success is true
    bool get_attenuator_enable(bool &value);
    
    /// @brief Set the corresponding power line
    void set_power_state(bool value);

    bool get_power_state(bool &value);
};
