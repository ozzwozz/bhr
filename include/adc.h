#pragma once

#include "pico/stdlib.h"
#include "hardware/adc.h"

class ADC
{
private:
    float ADC1_CTRL_value;
    float P5V5_value;
    float P3V3_value;
    float OCXO_PGOOD_value;
    float P12V_value;
    float INTERNAL_value;

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
    bool ADC::read_all();
};
