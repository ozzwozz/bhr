#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"

/// @class SI53361 - Low Jitter Fanout Clock
/// @brief SI53361 Driver
class SI53361
{
private:
    /// @param m_clock_select_pin clock select pin
    uint m_clock_select_pin = 19;
    /// @param m_output_enable_pin output enable pin
    uint m_output_enable_pin = 20;

public:
    /// @brief Construct a new SI53361 object
    SI53361();

    /// @brief Destroy the SI53361 object
    ~SI53361();

    /// @brief Enable the external clock
    void enable_external_clock();
    /// @brief Get the current clock state based on the enabeld pins
    uint8_t get_clock_state();
    /// @brief Enable the internal clock
    void enable_internal_clock();
    /// @brief Disable the clock
    void disable_clock();
};
