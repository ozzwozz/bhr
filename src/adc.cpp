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

void ADC::read_all()
{
    voltage_P5V5 = read_voltage_adc(P5V5_PIN);
    voltage_P3V3 = read_voltage_adc(P3V3_OCXO_PGOOD_PIN);
    voltage_P12V = read_voltage_adc(P12V_PIN);
    voltage_INTERNAL = read_voltage_adc(INTERNAL_PIN);

    gpio_put(ADC1_CTRL_value, 1);
    voltage_OCXO_PGOOD = read_voltage_adc(P3V3_OCXO_PGOOD_PIN);
    gpio_put(ADC1_CTRL_value, 0);
}