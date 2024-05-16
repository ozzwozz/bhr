#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"

class SI53361
{
    private:
        uint m_clock_select_pin = 19;
        uint m_output_enable_pin = 20;

    public:
        SI53361(/* args */);
        ~SI53361();

        void output_enable();
        void clk_sel();
        void enable_external_clock();
        void enable_internal_clock();
        void disable_clock();
        
};