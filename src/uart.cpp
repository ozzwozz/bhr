#include "uart.h"

UART::UART(uart_inst_t *uart, uint baud_rate, uint rx_pin, uint tx_pin) : m_uart(uart)
{
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    uart_set_baudrate(m_uart, baud_rate);
    uart_set_format(m_uart, 8, 1, UART_PARITY_NONE);

    // Turn off FIFO's - we want to do this character by character
    uart_set_fifo_enabled(m_uart, false);

    // Set UART flow control CTS/RTS, we don't want these, so turn them off
    uart_set_hw_flow(m_uart, false, false);

    uart_set_irq_enables(m_uart, true, false);
    
    
    int UART_IRQ = uart == m_uart ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, (irq_handler_t)uart_irq_handler);
    irq_set_enabled(UART_IRQ, true);
}

void UART::write(const char *data)
{
    while (*data != '\0')
    {
        uart_putc(m_uart, *data++);
    }
}

void UART::write(const char *data, size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        uart_putc(m_uart, data[i]);
    }
}

int UART::read(char *data, size_t len)
{
    int count = 0;
    while (len--)
    {
        int c = uart_getc(m_uart);
        if (c == PICO_ERROR_TIMEOUT)
        {
            break;
        }
        *data++ = static_cast<char>(c);
        ++count;
    }
    return count;
}

int UART::available()
{
    return uart_is_readable(m_uart) ? 1 : 0;
}

// RX interrupt handler
void UART::uart_irq_handler(void *context)
{
    UART *uart = static_cast<UART *>(context);
    while (uart_is_readable(uart->m_uart))
    {
        uint8_t ch = uart_getc(uart->m_uart);
        // Can we send it back?
        if (uart_is_writable(uart->m_uart)) {
            // Change it slightly first!
            ch++;
            uart_putc(uart->m_uart, ch);
        }
    }
}