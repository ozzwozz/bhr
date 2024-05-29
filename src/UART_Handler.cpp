#include "UART_Handler.h"
#include <string.h>
#include <stdio.h>

UART_Handler::UART_Handler(MAX31725 &max31725
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

    // Set up interrupt for GPIO 15
    irq_set_exclusive_handler(15, (irq_handler_t)ext_trig_irq_handler);
}

void UART_Handler::ext_trig_irq_handler(void *context)
{
    // Send interrupt occured message to the SBC
    printf("%d", (char*)EXTERNAL_INTERRUPT);
}

void UART_Handler::decode_message(const uint8_t message[3])
{
    char data[128]; // Allocate memory for the data buffer

    printf("message: %s\n", message);

    uint8_t header = message[0];
    uint8_t band_mask = message[1];

    std::vector<char> response;

    response[0] = header;

    switch (header)
    {
        case message_headers::SET_ATTENUATION:
            set_attenuation(response, data);
            break;
        case message_headers::GET_ATTENUATION:
            get_attenuation(response, band_mask);
            break;
        case message_headers::SET_LNA_ENABLE:
            set_lna_enable(response, data);
            break;
        case message_headers::GET_LNA_ENABLE:
            get_lna_enable(response);
            break;
        case message_headers::SET_ATTENUATOR_ENABLE:
            set_attenuator_enable(response, data);
            break;
        case message_headers::GET_ATTENUATOR_ENABLE:
            get_attenuator_enable(response);
            break;
        case message_headers::SET_CALIBRATION:
            set_calibration(data);
            break;
        case message_headers::GET_CALIBRATION:
            get_calibration(response);
            break;
        case message_headers::GET_BITS:
            get_bits(response);
            break;
        case message_headers::GET_HARDWARE_NUMBERS:
            get_hardware_numbers(response);
            break;
        case message_headers::GET_SOFTWARE_NUMBERS:
            get_software_numbers(response);
            break;
        default:
            break;
    }

    printf("%s", response);
}

void UART_Handler::set_attenuation(std::vector<char>& response, char* data)
{
    uint8_t attenuation_value = data[1];
    uint8_t band_mask = data[2];
    uint8_t set_attenuators = 0;

    if ((band_mask & (1 << 7)) != 0)
    {
        set_attenuators |= (m_pca9554_1.set_outputs(attenuation_value) << 7);
    }
    if ((band_mask & (1 << 6)) != 0)
    {
        set_attenuators |= (m_pca9554_2.set_outputs(attenuation_value) << 6);
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        set_attenuators |= (m_pca9554_3.set_outputs(attenuation_value) << 5);
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        set_attenuators |= (m_pca9554_4.set_outputs(attenuation_value) << 4);
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        set_attenuators |= (m_pca9554_5.set_outputs(attenuation_value) << 3);
    }

    response[1] = set_attenuators;
}

void UART_Handler::get_attenuation(std::vector<char>& response, uint8_t band_mask)
{
    uint8_t attenuation_value;
    uint8_t temp;


    if ((band_mask & (1 << 7)) != 0)
    {
        if (m_pca9554_1.read_inputs(temp))
        {
            attenuation_value |= temp;
        }    
    }
    else if ((band_mask & (1 << 6)) != 0)
    {
        if (m_pca9554_2.read_inputs(temp))
        {
            attenuation_value |= temp;
        }
    }
    else if ((band_mask & (1 << 5)) != 0)
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
    else if ((band_mask & (1 << 3)) != 0)
    {
        if (m_pca9554_5.read_inputs(temp))
        {
            attenuation_value |= temp;
        }    
    }

    response[1] = attenuation_value;
}

void UART_Handler::set_lna_enable(std::vector<char>& response, char* data)
{
    uint8_t lna_enabled = data[1];
    uint8_t band_mask = data[2];
    uint8_t enabled_lna;

    if ((band_mask & (1 << 7)) != 0)
    {
        enabled_lna |= (m_pca9554_1.set_lna(lna_enabled) << 7);
    }
    if ((band_mask & (1 << 6)) != 0)
    {
        enabled_lna |= (m_pca9554_2.set_lna(lna_enabled) << 6);
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        enabled_lna |= (m_pca9554_3.set_lna(lna_enabled) << 5);
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        enabled_lna |= (m_pca9554_4.set_lna(lna_enabled) << 4);
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        enabled_lna |= (m_pca9554_5.set_lna(lna_enabled) << 3);
    }
    
    response[1] = enabled_lna;
}

void UART_Handler::get_lna_enable(std::vector<char>& response)
{
    uint8_t lna_status;
    bool value;

    if (m_pca9554_1.get_lna(value))
    {
        lna_status |= (value << 7);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 6);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 5);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 4);
    }
    if (m_pca9554_2.get_lna(value))
    {
        lna_status |= (value << 3);
    }
    
    response[1] = lna_status;
}

void UART_Handler::set_attenuator_enable(std::vector<char>& response, char* data)
{
    uint8_t attenuator_enabled = data[1];
    uint8_t band_mask = data[2];
    uint8_t enabled_attenuators;

    if ((band_mask & (1 << 7)) != 0)
    {
        enabled_attenuators |= (m_pca9554_1.set_attenuator_enable(attenuator_enabled) << 7);
    }
    if ((band_mask & (1 << 6)) != 0)
    {
        enabled_attenuators |= (m_pca9554_2.set_attenuator_enable(attenuator_enabled) << 6);
    }
    if ((band_mask & (1 << 5)) != 0)
    {
        enabled_attenuators |= (m_pca9554_3.set_attenuator_enable(attenuator_enabled) << 5);
    }
    if ((band_mask & (1 << 4)) != 0)
    {
        enabled_attenuators |= (m_pca9554_4.set_attenuator_enable(attenuator_enabled) << 4);
    }
    if ((band_mask & (1 << 3)) != 0)
    {
        enabled_attenuators |= (m_pca9554_5.set_attenuator_enable(attenuator_enabled) << 3);
    }

    response[1] = enabled_attenuators;
}

void UART_Handler::get_attenuator_enable(std::vector<char>& response)
{
    uint8_t attenuators_enabled;
    bool value;

    if (m_pca9554_1.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 7);
    }
    if (m_pca9554_2.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 6);
    }
    if (m_pca9554_3.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 5);
    }
    if (m_pca9554_4.get_attenuator_enable(value))
    {
    attenuators_enabled |= (value << 4);
    }
    if (m_pca9554_5.get_attenuator_enable(value))
    {
        attenuators_enabled |= (value << 3);
    }
    
    response[1] = attenuators_enabled;
}

void UART_Handler::set_calibration(char* data)
{
    uint8_t calibration_table = data[1];
    uint8_t band_mask = data[2];
}

void UART_Handler::get_calibration(std::vector<char>& response)
{
    // unknown return
}

void UART_Handler::get_bits(std::vector<char>& response)
{
    uint32_t timestamp;
    float temperature;

    size_t initialSize = response.size(); // Store initial size to calculate the size of added data

    // Get timestamp from DS1682
    if (m_ds1682.getTime(timestamp))
    {
        response.resize(initialSize + sizeof(uint32_t));
        memcpy(response.data() + initialSize, &timestamp, sizeof(uint32_t));
    }

    // Get temperature from MAX31725
    if (m_max31725.read_temperature(temperature))
    {
        response.resize(initialSize + sizeof(float));
        memcpy(response.data() + initialSize, &temperature, sizeof(float));
    }
}

void UART_Handler::get_hardware_numbers(std::vector<char>& response)
{
    uint32_t device_id;

    // ETR Hardware Number
    if (!m_ds1682.getUniqueID(device_id))
    {
        printf("Unable to retrieve the hardware numbers.");
    }

    // EEPROM (?)
}

void UART_Handler::get_software_numbers(std::vector<char>& response)
{
    // Pi Pico Software Number
}
