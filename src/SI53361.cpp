#include "SI53361.h"

SI53361::SI53361()
{
    gpio_init(m_clock_select_pin);
    gpio_init(m_output_enable_pin);

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

void SI53361::enable_internal_clock()
{
    gpio_put(m_output_enable_pin, 1); // Enable Clock// Turn LED on
    gpio_put(m_clock_select_pin, 0);  // Select the internal clock
}