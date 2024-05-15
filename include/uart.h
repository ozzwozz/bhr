#pragma once

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <queue>

/**
 * @file UART Driver
 */
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

        size_t read(char *data, size_t len);
        int available();
        void flush_rx();
        void flush_tx();

        size_t send_message();

    private:
        /** @param m_uart uart instance*/
        uart_inst_t *m_uart;
        /** @param m_rx_pin rx pin*/
        uint m_rx_pin;
        /** @param m_tx_pin tx pin*/
        uint m_tx_pin;
        /** @param rx_buffer_ rx buffer queue*/
        std::queue<char> rx_buffer_;
        /** @param tx_buffer_ tx buffer queue*/
        std::queue<char> tx_buffer_;

        static void ext_trig_irq_handler(void *context);
        static void uart_irq_handler(void *context);
        void decode_message();

        uint8_t set_attenuation(char* data);
        void get_attenuation(const char* response);
        uint8_t set_lna_enable(char* data);
        void get_lna_enable(const char* response);
        uint8_t set_attenuator_enable(char* data);
        void get_attenuator_enable(const char* response);
        void set_calibration(char* data);
        void get_calibration(const char* response);
        void get_bits(const char* response);
        void get_hardware_numbers(const char* response);
        void get_software_numbers(const char* response);
};
