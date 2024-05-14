#pragma once

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"


class UART
{
    public:
        UART(uart_inst_t *uart, uint baud_rate, uint rx_pin, uint tx_pin);

        void write(const char *data);
        void write(const char *data, size_t len);

        int read(char *data, size_t len);
        int available();

    private:
        uart_inst_t *m_uart;
        uint m_rx_pin;
        uint m_tx_pin;

        static void uart_irq_handler(void *context);
};