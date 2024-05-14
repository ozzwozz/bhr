#pragma once

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <queue>


class UART
{
    public:
        enum message 
        {
            SET_ATTENUATION = 0x00,
            GET_ATTENUATION = 0x01,
            SET_LNA_ENABLE = 0x02, 
            GET_LNA_ENABLE = 0x03,
            SET_ATTENUATOR_ENABLE = 0x04,
            GET_ATTENUATOR_ENABLE = 0x05,
            SET_CALIBRATION = 0x10,
            GET_CALIBRATION = 0x11,
            GET_BITS = 0X20,
            GET_HARDWARE_NUMBERS = 0X21,
            GET_SOFTWARE_NUMBERS = 0X22
        }; // Enum inside the class

        UART(uart_inst_t *uart, uint baud_rate, uint rx_pin, uint tx_pin);

        void write(const char *data);
        void write(const char *data, size_t len);

        bool read(char *data, size_t len);
        int available();
        void flush();

    private:
        uart_inst_t *m_uart;
        uint m_rx_pin;
        uint m_tx_pin;
        std::queue<char> rx_buffer_;

        static void uart_irq_handler(void *context);
        void decode_message();
};