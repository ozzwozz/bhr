#pragma once

#include "pico/stdlib.h"
#include "hardware/adc.h"

/// @class ADC
/// @brief ADC Driver for reading from analogue to digital pins
class ADC
{
private:
    /// @param ADC1_CTRL_value member variable to store the ADC1 control value
    float ADC1_CTRL_value;
    /// @param P5V5_value member variable to store the P5V5 voltage
    float P5V5_V;
    /// @param P3V3_value member variable to store the P3V3 voltage
    float P3V3_V;
    /// @param OCXO_PGOOD_V member variable to store the OCXO_PGOOD voltage
    float OCXO_PGOOD_V;
    /// @param P12V_value member variable to store the P12V voltage
    float P12V_V;
    /// @param INTERNAL_value member variable to store the INTERNAL voltage
    float INTERNAL_V;

public:
    /// @brief Construct a new ADC object
    ADC();
    /// @brief Destroy the ADC object
    ~ADC();

    /// @param ADC1_CTRL_PIN ADC 1 voltage Control Pin
    const uint ADC1_CTRL_PIN = 22;
    /// @param P5V5_PIN P5V5 voltage Pin
    const uint P5V5_PIN = 26;
    /// @param P3V3_OCXO_PGOOD_PIN P3V3 and OCXO voltage Pin
    const uint P3V3_OCXO_PGOOD_PIN = 27;
    /// @param P12V_PIN P12V voltage Pin
    const uint P12V_PIN = 28;
    /// @param INTERNAL_PIN Internal voltage Pin
    const uint INTERNAL_PIN = 29;

    /// @brief read the raw value of the given adc pin
    /// @param pin 
    /// @return uint16_t value on the pin
    uint16_t read_raw_adc(uint pin);

    /// @brief read the converted voltage value of the given adc pin
    /// @param pin 
    /// @return float the voltage value at the pin
    float read_voltage_adc(uint pin);

    /// @brief read the voltage on all of the pins
    void read_all();
};
