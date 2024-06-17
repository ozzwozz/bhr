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
    gpio_pull_up(m_ext_trig_pin); // Set External interrupt trigger as input with pull-down resistor

    // Set up interrupt for m_ext_trig_pin
    gpio_set_irq_enabled_with_callback(m_ext_trig_pin, GPIO_IRQ_EDGE_RISE, true, ext_trig_irq_handler); 

    // irq_set_exclusive_handler(USBCTRL_IRQ, (irq_handler_t)usb_rx);
    // irq_set_enabled(USBCTRL_IRQ, true);
}

void USB_Handler::ext_trig_irq_handler(uint gpio, uint32_t events)
{
    
    // Send interrupt occured message to the SBC
    // printf("%d", (char*)EXTERNAL_INTERRUPT);
}

void USB_Handler::decode_message(const uint8_t message[3])
{
    uint8_t mutable_message[3];
    uint8_t header = message[0];
    uint8_t band_mask = message[1];

    uint8_t response[12];

    mutable_message[0] = message[0];
    mutable_message[1] = message[1];
    mutable_message[2] = message[2];

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
            printf("%d \n", response[12]);
            break;
        case message_headers::GET_HARDWARE_NUMBERS:
            get_hardware_numbers(response);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
            break;
        case message_headers::GET_SOFTWARE_NUMBERS:
            get_software_numbers(response);
            printf("%x ", response[0]);
            printf("%d \n", response[1]);
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

void USB_Handler::set_attenuation(uint8_t response[12], uint8_t data[3])
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

void USB_Handler::get_attenuation(uint8_t response[12], uint8_t band_mask)
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

void USB_Handler::set_lna_enable(uint8_t response[12], uint8_t data[3])
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

void USB_Handler::get_lna_enable(uint8_t response[12])
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

void USB_Handler::set_attenuator_enable(uint8_t response[12], uint8_t data[3])
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

void USB_Handler::get_attenuator_enable(uint8_t response[12])
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

void USB_Handler::set_clock_state(uint8_t response[12], uint8_t data[3])
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

void USB_Handler::get_clock_state(uint8_t response[12])
{
    response[1] = m_si53361.get_clock_state();
}

void USB_Handler::set_calibration(uint8_t data[3])
{
    uint8_t calibration_table = data[1];
    uint8_t band_mask = data[2];
}

void USB_Handler::get_calibration(uint8_t response[12])
{
    // unknown return
}

void USB_Handler::get_bits(uint8_t response[12])
{
    uint32_t timestamp;
    uint16_t counter;
    float temperature;

    // Get timestamp from DS1682
    if (m_ds1682.getTime(timestamp))
    {
        response[1] = timestamp >> 24;
        response[2] = timestamp >> 16;
        response[3] = timestamp >> 8;
        response[4] = timestamp;
    }

    // Get timestamp from DS1682
    if (m_ds1682.getEventCounter(counter))
    {
        response[5] = timestamp >> 8;
        // Extract lower 8 bits
        response[6] = timestamp & 0xFF;
    }

    // Get temperature from MAX31725
    if (m_max31725.read_temperature(temperature))
    {
        response[7] = temperature;
    }

    m_adc.read_all();
    
    response[8] = m_adc.voltage_INTERNAL;
    response[9] = m_adc.voltage_OCXO_PGOOD;
    response[10] = m_adc.voltage_P12V;
    response[11] = m_adc.voltage_P5V5;
    response[12] = m_adc.voltage_P3V3;
}

void USB_Handler::get_hardware_numbers(uint8_t response[12])
{
    uint32_t device_id;

    // ETR Hardware Number
    if (!m_ds1682.getUniqueID(device_id))
    {
        // printf("Unable to retrieve the hardware numbers.");
    }
    response[1] = device_id;

    // EEPROM (?)
}

void USB_Handler::get_software_numbers(uint8_t response[12])
{
    // Pi Pico Software Number
    response[1] = 1;
}

void USB_Handler::reset_etc()
{
    m_ds1682.reset();
}

void USB_Handler::set_p3v3_oxco_pin(uint8_t mutable_message[3])
{
    if (mutable_message[1])
        m_adc.set_p3v3_oxco_enable_pin();
    else
        m_adc.set_p3v3_oxco_disable();
}

void USB_Handler::set_p5v5_pin(uint8_t mutable_message[3])
{
    if (mutable_message[1])
        m_adc.set_p5v5_enable_pin();
    else
        m_adc.set_p5v5_disable();
}