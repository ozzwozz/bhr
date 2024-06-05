#pragma once

#include "I2CDevice.h"


/// @class MAX31725
/// @brief Driver for the MAX31725 - Local Temperature Sensor Driver
class MAX31725 : public I2CDevice
{
public:
    /// @brief Construct a new MAX31725 object
    /// @param i2c i2c instance
    /// @param address Address of the target
    MAX31725(i2c_inst_t *i2c, uint8_t address);

    /// @brief Destroy the MAX31725 object
    ~MAX31725();

    /// @brief set the over_temp trigger temperature
    /// @return success
    bool set_over_temp_limit();

    /// @brief Read register storing the temperature data on the MAX31725
    /// @param temperature get the temperature by reference
    /// @return success is true
    bool read_temperature(float &temperature);

    /// @brief Handler for the over temp interrupt
    static void over_temp_irq_handler(uint gpio, uint32_t events);

private:
    /// @param m_cmd_read_temperature command address for reading the temperature
    static constexpr uint8_t  m_cmd_read_temperature = 0x00;

    /// @param m_cmd_set_temp_limit command address for setting the over_temp limit
    static constexpr uint8_t m_cmd_set_temp_limit = 0x03;

    /// @param m_overtemp_pin interrupt pin for the overtemp trigger
    const uint m_overtemp_pin = 6;

    /// @param m_temperature_limit_C over_temp trigger temperrature in Celsius
    const float m_temperature_limit_C = 85;
};
