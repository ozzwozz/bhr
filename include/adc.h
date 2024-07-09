#pragma once

#include "pico/stdlib.h"
#include "hardware/adc.h"

/// @class ADC
/// @brief ADC Driver for reading from analogue to digital pins
class ADC
{
private:
    /// @param P5V5_ENABLE_PIN P5V5 voltage Pin
    const uint P5V5_ENABLE_PIN = 12;
    /// @param P3V3_OCXO_ENABLE_PIN P3V3 and OCXO voltage Pin
    const uint P3V3_OCXO_ENABLE_PIN = 14;

    /// @param P5V5_PGOOD_PIN P5V5 voltage Pin
    const uint P5V5_PGOOD_PIN = 13;
    /// @param P3V3_OCXO_PGOOD_PIN P3V3 and OCXO voltage Pin
    const uint P3V3_OCXO_PGOOD_PIN = 21;

    /// @param ADC1_CTRL_PIN ADC 1 voltage Control Pin
    const uint ADC1_CTRL_PIN = 22;

    /// @param P5V5_PIN P5V5 voltage Pin
    const uint P5V5_PIN = 26;
    const uint P5V5_PIN_CHANNEL = 0;
    /// @param P3V3_OCXO_PIN P3V3 and OCXO voltage Pin
    const uint P3V3_OCXO_PIN = 27;
    const uint P3V3_OCXO_PIN_CHANNEL = 1;
    /// @param P12V_PIN P12V voltage Pin
    const uint P12V_PIN = 28;
    const uint P12V_PIN_CHANNEL = 2;

public:
    /// @brief Construct a new ADC object
    ADC();
    /// @brief Destroy the ADC object
    ~ADC();
    
    /// @param ADC1_CTRL_value member variable to store the ADC1 control value
    uint16_t ADC1_CTRL_value;
    /// @param voltage_P5V5 member variable to store the P5V5 voltage
    uint16_t voltage_P5V5;
    /// @param voltage_P3V3 member variable to store the P3V3 voltage
    uint16_t voltage_P3V3;
    /// @param voltage_OCXO_PGOOD member variable to store the OCXO_PGOOD voltage
    uint16_t voltage_OCXO_PGOOD;
    /// @param voltage_P12V member variable to store the P12V voltage
    uint16_t voltage_P12V;
    /// @param voltage_P3V3_PGOOD member variable to store the p3v3 voltage
    uint16_t voltage_P3V3_PGOOD;
    /// @param voltage_P5V5_PGOOD member variable to store the p5v5 voltage
    uint16_t voltage_P5V5_PGOOD;

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

    /// @brief Set the p5v5 enable pin
    void set_p5v5_enable_pin();

    /// @brief Set the p3v3 oxco enable pin
    void set_p3v3_oxco_enable_pin();

    /// @brief Unset the p5v5 enable pin
    void set_p5v5_disable();

    /// @brief Unset the p3v3 oxco enable pin
    void set_p3v3_oxco_disable();
};
