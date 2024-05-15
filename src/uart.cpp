#include "uart.h"
#include <string.h>
#include <stdio.h>

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


    // Initialize GPIO 15
    gpio_init(15);
    gpio_set_dir(15, GPIO_IN);
    gpio_pull_down(15); // Set GPIO 15 as input with pull-down resistor

    // Set up interrupt for GPIO 15
    irq_set_exclusive_handler(15, (irq_handler_t)ext_trig_irq_handler);

}

void UART::write(const char *data)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), strlen(data));
}

void UART::write(const char *data, size_t len)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), len);
}

bool UART::read(char *data, size_t len)
{
    size_t bytes_read = 0;
    while (!rx_buffer_.empty() && bytes_read < len)
    {
        data[bytes_read++] = rx_buffer_.front();
        rx_buffer_.pop();
    }
    return bytes_read > 0;
}

int UART::available()
{
    return uart_is_readable(m_uart) ? 1 : 0;
}

void UART::flush()
{
    rx_buffer_ = std::queue<char>();
}

void UART::ext_trig_irq_handler(void *context)
{
    UART *uart = static_cast<UART *>(context);
    uart->write("GPIO 15 interrupt triggered!\n");

    // Your interrupt handler code here
    // This will be called when the interrupt is triggered on GPIO 15
    for (int i = 0; i < 10; i++)
    {
        gpio_put(PICO_DEFAULT_LED_PIN, 1); // Turn LED on
        sleep_ms(500);
        gpio_put(PICO_DEFAULT_LED_PIN, 0); // Turn LED off
        sleep_ms(500);
    }

}

// RX interrupt handler
void UART::uart_irq_handler(void *context)
{
    UART *uart = static_cast<UART *>(context);

    while (uart_is_readable(uart->m_uart))
    {
        char c = uart_getc(uart->m_uart);
        uart->rx_buffer_.push(c);
    }
}

void UART::decode_message()
{
    char data[128]; // Allocate memory for the data buffer

    if (!read(data, sizeof(data)))
    {
        return;
    }

    printf("message: %s\n", data);

    uint8_t header = data[0];

    switch (header)
    {
        case message::SET_ATTENUATION:
            ;
            break;
        case message::GET_ATTENUATION:
            ;
            break;
        case message::SET_LNA_ENABLE:
            ;
            break;
        case message::GET_LNA_ENABLE:
            ;
            break;
        case message::SET_ATTENUATOR_ENABLE:
            ;
            break;
        case message::GET_ATTENUATOR_ENABLE:
            ;
            break;
        case message::SET_CALIBRATION:
            ;
            break;
        case message::GET_CALIBRATION:
            ;
            break;
        case message::GET_BITS:
            ;
            break;
        case message::GET_HARDWARE_NUMBERS:
            ;
            break;
        case message::GET_SOFTWARE_NUMBERS:
            ;
            break;
        default:
            break;
    }
}