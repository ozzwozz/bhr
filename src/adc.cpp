#include "adc.h"

ADC::ADC()
{
    adc_init();
    adc_gpio_init(ADC1_CTRL_PIN);
    adc_gpio_init(P5V5_PIN);
    adc_gpio_init(P3V3_OCXO_PGOOD_PIN);
    adc_gpio_init(P12V_PIN);
    adc_gpio_init(INTERNAL_PIN);
}

ADC::~ADC()
{
}

uint16_t ADC::read_raw_adc(uint pin)
{
    adc_select_input(pin);
    return adc_read();
}

float ADC::read_voltage_adc(uint pin)
{
    adc_select_input(pin);
    uint16_t adc_value = read_raw_adc(pin);
    return (3.3 * adc_value) / (1 << 12);
}

bool ADC::read_all()
{
    P5V5_value = read_voltage_adc(P5V5_PIN);
    P3V3_value = read_voltage_adc(P3V3_OCXO_PGOOD_PIN);
    P12V_value = read_voltage_adc(P12V_PIN);
    INTERNAL_value = read_voltage_adc(INTERNAL_PIN);

    gpio_put(ADC1_CTRL_value, 1);
    OCXO_PGOOD_value = read_voltage_adc(P3V3_OCXO_PGOOD_PIN);
    gpio_put(ADC1_CTRL_value, 0);
}