#include "USB_Handler.h"
#include <string.h>
#include <stdio.h>
#include "tusb.h"
#include <SI53361.h>

USB_Handler::USB_Handler(MAX31725 &max31725
        , M24M02 &m24m02, SI53361 &si53361, PCA9554 &pca9554_1, PCA9554 &pca9554_2,
        PCA9554 &pca9554_3, PCA9554 &pca9554_4, PCA9554 &pca9554_5, ADC &adc, DS1682 &ds1682)
            : m_max31725(max31725)
            , m_m24m02(m24m02)
            , m_si53361(si53361)
            , m_pca9554_1(pca9554_1)
            , m_pca9554_2(pca9554_2)
            , m_pca9554_3(pca9554_3)
            , m_pca9554_4(pca9554_4)
            , m_pca9554_5(pca9554_5)
            , m_adc(adc)
            , m_ds1682(ds1682)
{
    // Initialize External interrupt trigger
    gpio_init(m_ext_trig_pin);
    gpio_set_dir(m_ext_trig_pin, GPIO_IN);
    gpio_pull_down(m_ext_trig_pin); // Set External interrupt trigger as input with pull-down resistor
    // Set up interrupt for m_ext_trig_pin
    gpio_set_irq_enabled_with_callback(m_ext_trig_pin, GPIO_IRQ_EDGE_RISE, true, (gpio_irq_callback_t)gpio_callback);

    // Initialize External interrupt trigger
    gpio_init(m_overtemp_pin);
    gpio_set_dir(m_overtemp_pin, GPIO_IN);
    gpio_pull_up(m_overtemp_pin); // Set External interrupt trigger as input with pull-down resistor

    // Set up interrupt for m_overtemp_pin
    gpio_set_irq_enabled_with_callback(m_overtemp_pin, GPIO_IRQ_EDGE_FALL, true, (gpio_irq_callback_t)gpio_callback);

}

void USB_Handler::gpio_callback(void *context)
{
    USB_Handler* instance = static_cast<USB_Handler*>(context);
    if (gpio_get(instance->m_overtemp_pin))
    {
        instance->m_overtemp_flag = true;       
    }
    if (gpio_get(instance->m_overtemp_pin))
    {
        instance->m_ext_trig_flag = true;
    }
}

bool USB_Handler::get_ext_trig_flag()
{
    return m_ext_trig_flag;
}

bool USB_Handler::get_overtemp_flag()
{
    return m_overtemp_flag;
}

void USB_Handler::resolve_ext_trig_flag()
{
    if (printf("%x \n", EXTERNAL_INTERRUPT) > 0)
    {
        m_ext_trig_flag = 0;
    }
}

void USB_Handler::resolve_overtemp_flag()
{
    if (printf("%x \n", OVERTEMP_INTERRUPT) > 0)
    {
        m_overtemp_flag = 0;
    }
}

void USB_Handler::decode_message(const uint8_t message[5])
{
    uint8_t mutable_message[5];
    uint8_t header = message[0];
    uint8_t band_mask = message[1];

    uint8_t response[20];

    mutable_message[0] = message[0];
    mutable_message[1] = message[1];
    mutable_message[2] = message[2];
    mutable_message[3] = message[3];
    mutable_message[4] = message[4];

    response[0] = header;

    switch (header)
    {
        case message_headers::SET_ATTENUATION:
            set_attenuation(response, mutable_message);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::GET_ATTENUATION:
            get_attenuation(response, band_mask);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::SET_LNA_ENABLE:
            set_lna_enable(response, mutable_message);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::GET_LNA_ENABLE:
            get_lna_enable(response);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::SET_ATTENUATOR_ENABLE:
            set_attenuator_enable(response, mutable_message);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::GET_ATTENUATOR_ENABLE:
            get_attenuator_enable(response);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::SET_10MHZ_REFERENCE_SOURCE:
            set_clock_state(response, mutable_message);
            printf("%x ", response[0]);
            printf("%x \n", response[1]);
            break;
        case message_headers::GET_10MHZ_REFERENCE_SOURCE:
            get_clock_state(response);
            printf("%x ", response[0]);
            printf("%x \n", response[1]);
            break;
        case message_headers::SET_CALIBRATION:
            set_calibration(mutable_message);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::GET_CALIBRATION:
            get_calibration(response);
            break;
        case message_headers::GET_BITS:
            get_bits(response);
            printf("%x ", response[0]);
            printf("%d ", response[1]);
            printf("%d ", response[2]);
            printf("%d ", response[3]);
            printf("%d ", response[4]);
            printf("%d ", response[5]);
            printf("%d ", response[6]);
            printf("%d ", response[7]);
            printf("%d ", response[8]);
            printf("%d ", response[9]);
            printf("%d ", response[10]);
            printf("%d ", response[11]);
            printf("%d ", response[12]);
            printf("%d ", response[13]);
            printf("%d ", response[14]);
            printf("%d ", response[15]);
            printf("%d ", response[16]);
            printf("%d ", response[17]);
            printf("%d ", response[18]);
            printf("%d \n", response[19]);
            break;
        case message_headers::GET_HARDWARE_NUMBERS:
            get_hardware_numbers(response);
            printf("%x ", response[0]);
            printf("%d ", response[1]);
            printf("%d ", response[2]);
            printf("%d ", response[3]);
            printf("%d \n", response[4]);
            break;
        case message_headers::GET_SOFTWARE_NUMBERS:
            get_software_numbers(response);
            printf("%x ", response[0]);
            printf("%d ", response[1]);
            printf("%d ", response[2]);
            printf("%d ", response[3]);
            printf("%d \n", response[4]);
            break;
        case message_headers::SET_HARDWARE_NUMBERS:
            set_hardware_numbers(response, mutable_message);
            printf("%x \n", response[0]);
            // printf("%d \n", response[1]);
            break;
        case message_headers::SET_SOFTWARE_NUMBERS:
            set_software_numbers(response, mutable_message);
            printf("%x \n", response[0]);
            // printf("%d \n", response[1]);
            break;
        case message_headers::RESET_ETC:
            reset_etc();
            printf("%x \n", header);
        case message_headers::SET_P3V3_OXCO_PIN:
            set_p3v3_oxco_pin(mutable_message);
            printf("%x \n", header);
        case message_headers::SET_P5V5_PIN:
            set_p5v5_pin(mutable_message);
            printf("%x \n", header);
        default:
            break;
    }
}

void USB_Handler::set_attenuation(uint8_t response[20], uint8_t data[5])
{
    uint8_t attenuation_value = data[1];
    uint8_t band_mask = data[2];
    uint8_t set_attenuators = 0;

    if ((band_mask & (1 << 1)) != 0)
    {
        set_attenuators |= (m_pca9554_1.set_outputs(attenuation_value) << 1);
    }
    if ((band_mask & (1 << 2)) != 0)
    {
        set_attenuators |= (m_pca9554_2.set_outputs(attenuation_value) << 2);
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        set_attenuators |= (m_pca9554_3.set_outputs(attenuation_value) << 3);
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        set_attenuators |= (m_pca9554_4.set_outputs(attenuation_value) << 4);
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        set_attenuators |= (m_pca9554_5.set_outputs(attenuation_value) << 5);
    }

    response[1] = set_attenuators;
}

void USB_Handler::get_attenuation(uint8_t response[20], uint8_t band_mask)
{
    uint8_t attenuation_value = 0;
    uint8_t temp;


    if ((band_mask & (1 << 1)) != 0)
    {
        if (m_pca9554_1.read_inputs(temp))
        {
            attenuation_value |= temp;
        }    
    }
    else if ((band_mask & (1 << 2)) != 0)
    {
        if (m_pca9554_2.read_inputs(temp))
        {
            attenuation_value |= temp;
        }
    }
    else if ((band_mask & (1 << 3)) != 0)
    {
        if (m_pca9554_3.read_inputs(temp))
        {
            attenuation_value |= temp;
        }
    }
    else if ((band_mask & (1 << 4)) != 0)
    {
        if (m_pca9554_4.read_inputs(temp))
        {
            attenuation_value |= temp;
        }
    }
    else if ((band_mask & (1 << 5)) != 0)
    {
        if (m_pca9554_5.read_inputs(temp))
        {
            attenuation_value |= temp;
        }    
    }

    response[1] = attenuation_value;
}

void USB_Handler::set_lna_enable(uint8_t response[20], uint8_t data[5])
{
    uint8_t lna_enabled = data[1];
    uint8_t band_mask = data[2];
    uint8_t enabled_lna = 0;

    if ((band_mask & (1 << 1)) != 0)
    {
        enabled_lna |= (m_pca9554_1.set_lna(lna_enabled) << 1);
    }
    if ((band_mask & (1 << 2)) != 0)
    {
        enabled_lna |= (m_pca9554_2.set_lna(lna_enabled) << 2);
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        enabled_lna |= (m_pca9554_3.set_lna(lna_enabled) << 3);
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        enabled_lna |= (m_pca9554_4.set_lna(lna_enabled) << 4);
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        enabled_lna |= (m_pca9554_5.set_lna(lna_enabled) << 5);
    }
    
    response[1] = enabled_lna;
}

void USB_Handler::get_lna_enable(uint8_t response[20])
{
    uint8_t lna_status = 0;
    bool value;

    if (m_pca9554_1.get_lna(value))
    {
        lna_status |= (value << 1);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 2);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 3);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 4);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 5);
    }
    
    response[1] = lna_status;
}

void USB_Handler::set_attenuator_enable(uint8_t response[20], uint8_t data[5])
{
    uint8_t attenuator_enabled = data[1];
    uint8_t band_mask = data[2];
    uint8_t enabled_attenuators = 0;

    if ((band_mask & (1 << 1)) != 0)
    {
        enabled_attenuators |= (m_pca9554_1.set_attenuator_enable(attenuator_enabled) << 1);
    }
    if ((band_mask & (1 << 2)) != 0)
    {
        enabled_attenuators |= (m_pca9554_2.set_attenuator_enable(attenuator_enabled) << 2);
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        enabled_attenuators |= (m_pca9554_3.set_attenuator_enable(attenuator_enabled) << 3);
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        enabled_attenuators |= (m_pca9554_4.set_attenuator_enable(attenuator_enabled) << 4);
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        enabled_attenuators |= (m_pca9554_5.set_attenuator_enable(attenuator_enabled) << 5);
    }

    response[1] = enabled_attenuators;
}

void USB_Handler::get_attenuator_enable(uint8_t response[20])
{
    uint8_t attenuators_enabled = 0;
    bool value;

    if (m_pca9554_1.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 1);
    }
    if (m_pca9554_2.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 2);
    }
    if (m_pca9554_3.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 3);
    }
    if (m_pca9554_4.get_attenuator_enable(value))
    {
    attenuators_enabled |= (value << 4);
    }
    if (m_pca9554_5.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 5);
    }
    
    response[1] = attenuators_enabled;
}

void USB_Handler::set_clock_state(uint8_t response[20], uint8_t data[5])
{
    if (data[1] == 0x01)
    {
        m_si53361.enable_external_clock();
        response[1] = 0x01;
    }
    else
    {
        m_si53361.enable_internal_clock();
        response[1] = 0x00;
    }

}

void USB_Handler::get_clock_state(uint8_t response[20])
{
    response[1] = m_si53361.get_clock_state();
}

void USB_Handler::set_calibration(uint8_t data[5])
{
    uint8_t calibration_table = data[1];
    uint8_t band_mask = data[2];
}

void USB_Handler::get_calibration(uint8_t response[20])
{
    // unknown return
}

void USB_Handler::get_bits(uint8_t response[20])
{
    uint32_t timestamp;
    uint16_t counter;
    float temperature;

    // Get timestamp from DS1682
    if (m_ds1682.getTime(timestamp))
    {
        printf("from pico %ld", timestamp);
        response[1] = (timestamp >> 24);
        response[2] = (timestamp >> 16);
        response[3] = (timestamp >> 8);
        response[4] = timestamp & 0xFF;
    }

    // Get timestamp from DS1682
    if (m_ds1682.getEventCounter(counter))
    {
        response[5] = counter >> 8;
        // Extract lower 8 bits
        response[6] = counter & 0xFF;
    }

    // Get temperature from MAX31725
    if (m_max31725.read_temperature(temperature))
    {
        response[7] = temperature;
    }

    m_adc.read_all();
    
    response[8] = (m_adc.voltage_P5V5_PGOOD >> 8);
    response[9] = m_adc.voltage_P5V5_PGOOD & 0xFF;

    response[10] = (m_adc.voltage_P3V3_PGOOD >> 8);
    response[11] = m_adc.voltage_P3V3_PGOOD & 0xFF;

    response[12] = (m_adc.voltage_OCXO_PGOOD >> 8);
    response[13] = m_adc.voltage_OCXO_PGOOD & 0xFF;

    response[14] = (m_adc.voltage_P12V >> 8);
    response[15] = m_adc.voltage_P12V & 0xFF;

    response[16] = (m_adc.voltage_P5V5 >> 8);
    response[17] = m_adc.voltage_P5V5 & 0xFF;

    response[18] = (m_adc.voltage_P3V3 >> 8);
    response[19] = m_adc.voltage_P3V3 & 0xFF;  
}

void USB_Handler::get_hardware_numbers(uint8_t response[20])
{
    uint32_t hardware_id;

    // EEPROM Hardware Number
    if (!m_m24m02.get_hardware_id(hardware_id))
    {
        // printf("Unable to retrieve the Software numbers.");
    }

    response[1] = (hardware_id >> 24);
    response[2] = (hardware_id >> 16);
    response[3] = (hardware_id >> 8);
    response[4] = hardware_id & 0xFF;
}

void USB_Handler::get_software_numbers(uint8_t response[20])
{
    uint32_t software_id;

    // EEPROM Software Number
    if (!m_m24m02.get_software_id(software_id))
    {
        // printf("Unable to retrieve the Software numbers.");
    }
    response[1] = (software_id >> 24);
    response[2] = (software_id >> 16);
    response[3] = (software_id >> 8);
    response[4] = software_id & 0xFF;
}

void USB_Handler::set_hardware_numbers(uint8_t response[20], uint8_t mutable_message[5])
{
    uint32_t hardware_id = (mutable_message[1] << 24) | (mutable_message[2] << 16) | (mutable_message[3] << 8) | (mutable_message[4] & 0xFF);
    m_m24m02.set_hardware_id(hardware_id);
}

void USB_Handler::set_software_numbers(uint8_t response[20], uint8_t mutable_message[5])
{
    uint32_t software_id = (mutable_message[1] << 24) | (mutable_message[2] << 16) | (mutable_message[3] << 8) | (mutable_message[4] & 0xFF);
    m_m24m02.set_software_id(software_id);
}

void USB_Handler::reset_etc()
{
    m_ds1682.reset();
}

void USB_Handler::set_p3v3_oxco_pin(uint8_t mutable_message[5])
{
    if (mutable_message[1])
    {
        m_adc.set_p3v3_oxco_enable_pin();
    }
    else
    {
        m_adc.set_p3v3_oxco_disable();
    }
}

void USB_Handler::set_p5v5_pin(uint8_t mutable_message[5])
{
    if (mutable_message[1])
    {
        m_adc.set_p5v5_enable_pin();
    }
    else
    {
        m_adc.set_p5v5_disable();
    }
}
