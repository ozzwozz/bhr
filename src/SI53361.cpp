#include "SI53361.h"

SI53361::SI53361()
{
    gpio_init(m_clock_select_pin);
    gpio_init(m_output_enable_pin);
    gpio_init(m_ext_clock_detect);

    // Default to using internal clock
    enable_internal_clock();
}

SI53361::~SI53361()
{

}

void SI53361::disable_clock()
{
    gpio_put(m_output_enable_pin, 0); // Disable Clock
}

void SI53361::enable_external_clock()
{
    gpio_put(m_output_enable_pin, 1); // Enable Clock
    gpio_put(m_clock_select_pin, 1);  // Select the external clock
}

uint8_t SI53361::get_clock_state()
{
    if (gpio_get(m_ext_clock_detect) == 1 )
    {
        return 0x01;
    }
    return 0x00;
}

void SI53361::enable_internal_clock()
{
    gpio_put(m_output_enable_pin, 1); // Enable Clock
    gpio_put(m_clock_select_pin, 0);  // Select the external clock
}
