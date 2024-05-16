#pragma once

#include "pico/stdlib.h"
#include "hardware/adc.h"

class ADC
{
private:
    uint16_t ADC1_CTRL_value;
    uint16_t P5V5_value;
    uint16_t P3V3_OCXO_PGOOD_value;
    uint16_t P12V_value;
public:
    ADC();
    ~ADC();

    uint ADC1_CTRL_PIN = 22;
    uint P5V5_PIN = 26;
    uint P3V3_OCXO_PGOOD_PIN = 27;
    uint P12V_PIN = 28;
    uint INTERNAL_PIN = 29;

    uint16_t read_raw_adc(uint pin);
    float read_voltage_adc(uint pin);
};
