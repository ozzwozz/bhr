#include "PCA9554.h"

PCA9554::PCA9554(i2c_inst_t *i2c, uint8_t address, uint power_enable_pin) : I2CDevice(i2c, address), m_power_enable_pin(power_enable_pin)
{
    gpio_init(m_power_enable_pin);
    gpio_set_dir(m_power_enable_pin, GPIO_OUT);
    
    set_power_state(1);
}

PCA9554::~PCA9554()
{
    set_power_state(0);
}

void PCA9554::configuration()
{
    // All pins as outputs
    uint8_t config = 0x00;
    uint8_t command[2] {config_register, config}; 
    int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
}

bool PCA9554::set_rf_path_value(uint16_t value)
{
    uint8_t value_per_attenuator;
    value_per_attenuator = value / 4;

    if (value_per_attenuator-(int)value_per_attenuator == 0
        || value_per_attenuator-(int)value_per_attenuator == 0.75
        || value_per_attenuator-(int)value_per_attenuator == 0.5
        || value_per_attenuator-(int)value_per_attenuator == 0.25)
    {
        for (int8_t x=0; x < 4; x++)
        {
            set_attenuator_value((1 << x), value_per_attenuator);
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool PCA9554::set_attenuator_value(uint8_t attenuator_id, uint8_t value)
{
    output_register.i = 0x00;

    if (!read_inputs(output_register.i))
    {
        return false;
    }
    
    output_register.b.attenuator_1 = 1;
    output_register.b.attenuator_2 = 1;
    output_register.b.attenuator_3 = 1;
    output_register.b.attenuator_4 = 1;
    
    if (attenuator_id & (1 << 0))
    {
        output_register.b.attenuator_1 = 0;
    }
    if (attenuator_id & (1 << 1))
    {
        output_register.b.attenuator_2 = 0;
    }
    if (attenuator_id & (1 << 2))
    {
        output_register.b.attenuator_3 = 0;
    }
    if (attenuator_id & (1 << 3))
    {
        output_register.b.attenuator_4 = 0;
    }

    uint8_t command[2] {output_port_register, output_register.i};

    // value = 0x0E;
    int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
    if (ret != PICO_ERROR_GENERIC)
    {
        for (int8_t x = 0; x < 8; x++)
        {
            output_register.b.data = (value & (1 << x));

            output_register.b.clock = 0x00;

            command[1] = output_register.i;
            ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
            if (ret == PICO_ERROR_GENERIC)
            {
                break;
            }
            output_register.b.clock = 1;
            // and here

            command[1] = output_register.i;
            ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
            if (ret == PICO_ERROR_GENERIC)
            {
                break;
            }
        }
    }

    output_register.b.clock = 0;
    output_register.b.data = 0;
    command[1] = output_register.i;

    ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    output_register.b.attenuator_1 = 1;
    output_register.b.attenuator_2 = 1;
    output_register.b.attenuator_3 = 1;
    output_register.b.attenuator_4 = 1;
    command[1] = output_register.i;

    ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    return true;
}

bool PCA9554::set_outputs(const uint8_t value)
{
    uint8_t current_value = 0;

    if (read_inputs(current_value))
    {

        // Masking currently set values so that only explicit changes are made
        uint8_t current_value_shifted = (current_value >> 6);
        uint8_t new_value = (current_value_shifted << 6) | value;

        uint8_t command[2] {output_port_register, current_value};

        int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
        if (ret == PICO_ERROR_GENERIC)
        {
            return false;
        }
    }

    return true;
}

bool PCA9554::read_inputs(uint8_t &value)
{
    if (!I2CDevice::write(&output_port_register, 1))
    {
        return false;
    }

    if (!I2CDevice::read(&value, 1))
    {
        return false;
    }
    
    return true;
}

bool PCA9554::set_lna(const bool value)
{
    uint8_t current_value = 0;

    if (read_inputs(current_value))
    {
        unsigned char clear_mask = ~(1 << 7);
        unsigned char set_mask = value << 6;

        current_value &= clear_mask;

        current_value |= set_mask;

        uint8_t command[2] {output_port_register, current_value};

        int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
        if (ret == PICO_ERROR_GENERIC)
        {
            return false;
        }
    }

    return true;
}

bool PCA9554::get_lna(bool &value)
{
    uint8_t current_value = 0;

    read_inputs(current_value);

    // Mask to clear the 7th bit
    value = current_value >> 6;

    return true;
}

bool PCA9554::set_attenuator_enable(bool value)
{
    uint8_t current_value = 0;

    read_inputs(current_value);

    unsigned char clear_mask = ~(1 << 6);
    unsigned char set_mask = value << 7;

    current_value &= clear_mask;

    current_value |= set_mask;
    
    uint8_t command[2] {output_port_register, current_value};

    int ret = i2c_write_blocking(m_i2c, m_address, command, 2, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    return true;
}

bool PCA9554::get_attenuator_enable(bool &value)
{
    uint8_t current_value = 0;

    read_inputs(current_value);

    value = (current_value >> 7);
    return true;
}

void PCA9554::set_power_state(bool enabled)
{
    gpio_put(m_power_enable_pin, enabled);
 
    if (enabled)
    {
        sleep_ms(100);
        configuration();
        set_attenuator_enable(1);
        set_outputs(0x3C);
    }
}

bool PCA9554::get_power_state()
{
    return gpio_get(m_power_enable_pin);
}
