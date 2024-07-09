#include "adc.h"

ADC::ADC()
{
    adc_init();
    adc_gpio_init(P5V5_PIN);
    adc_gpio_init(P3V3_OCXO_PIN);
    adc_gpio_init(P12V_PIN);
    gpio_set_input_enabled(P5V5_PIN, true);
    gpio_set_input_enabled(P3V3_OCXO_PIN, true);
    gpio_set_input_enabled(P12V_PIN, true);

    gpio_init(ADC1_CTRL_PIN);
    gpio_set_dir(ADC1_CTRL_PIN, GPIO_OUT);

    gpio_init(P5V5_PGOOD_PIN);
    gpio_init(P3V3_OCXO_PGOOD_PIN);
    gpio_set_dir(P5V5_PGOOD_PIN, GPIO_IN);
    gpio_set_dir(P3V3_OCXO_PGOOD_PIN, GPIO_IN);
    
    gpio_init(P5V5_ENABLE_PIN);
    gpio_init(P3V3_OCXO_ENABLE_PIN);
    gpio_set_dir(P5V5_ENABLE_PIN, GPIO_OUT);
    gpio_set_dir(P3V3_OCXO_ENABLE_PIN, GPIO_OUT);

    set_p5v5_enable_pin();
    set_p3v3_oxco_enable_pin();
}

ADC::~ADC()
{
    set_p5v5_disable();
    set_p3v3_oxco_disable();
}

uint16_t ADC::read_raw_adc(uint pin)
{
    adc_select_input(pin);
    return adc_read();
}

float ADC::read_voltage_adc(uint pin)
{
    uint16_t adc_value = read_raw_adc(pin);
    return (3.3 * adc_value) / (1 << 12);
}

void ADC::read_all()
{
    voltage_P5V5 = read_raw_adc(P5V5_PIN_CHANNEL);
    voltage_P3V3 = read_raw_adc(P3V3_OCXO_PIN_CHANNEL);
    voltage_P12V = read_raw_adc(P12V_PIN_CHANNEL);
    voltage_P5V5_PGOOD= gpio_get(P5V5_PGOOD_PIN);
    voltage_P3V3_PGOOD= gpio_get(P3V3_OCXO_PGOOD_PIN);

    gpio_put(ADC1_CTRL_value, 1);
    voltage_OCXO_PGOOD = read_raw_adc(P3V3_OCXO_PIN_CHANNEL);
    gpio_put(ADC1_CTRL_value, 0);
}

void ADC::set_p5v5_enable_pin()
{
    gpio_put(P5V5_ENABLE_PIN, 1);
}

void ADC::set_p3v3_oxco_enable_pin()
{
    gpio_put(P3V3_OCXO_ENABLE_PIN, 1);
}

void ADC::set_p5v5_disable()
{
    gpio_put(P5V5_ENABLE_PIN, 0);
}

void ADC::set_p3v3_oxco_disable()
{
    gpio_put(P3V3_OCXO_ENABLE_PIN, 0);
}
