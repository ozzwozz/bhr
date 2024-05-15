#include "uart.h"
#include <string.h>
#include <stdio.h>

/**
 * @brief Construct a new UART::UART object
 * 
 * @param uart 
 * @param baud_rate 
 * @param rx_pin 
 * @param tx_pin 
 */
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

/**
 * @brief Write to UART
 * 
 * @param data 
 */
void UART::write(const char *data)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), strlen(data));
}

/**
 * @brief Write to UART
 * 
 * @param data 
 * @param len 
 */
void UART::write(const char *data, size_t len)
{
    uart_write_blocking(m_uart, reinterpret_cast<const uint8_t*>(data), len);
}

/**
 * @brief Read from UART
 * 
 * @param data 
 * @param len 
 * @return size_t 
 */
size_t UART::read(char *data, size_t len)
{
    size_t bytes_read = 0;
    while (!rx_buffer_.empty() && bytes_read < len)
    {
        data[bytes_read++] = rx_buffer_.front();
        rx_buffer_.pop();
    }
    return bytes_read > 0;
}

/**
 * @brief Check if UART is available to interact with
 * 
 * @return int 
 */
int UART::available()
{
    return uart_is_readable(m_uart) ? 1 : 0;
}

/**
 * @brief flush the Rx buffer
 */
void UART::flush_rx()
{
    rx_buffer_ = std::queue<char>();
}

/**
 * @brief flush the Tx buffer
 */
void UART::flush_tx()
{
    tx_buffer_ = std::queue<char>();
}

/**
 * @brief Handler for the EXT_TRIG/GPIO15 interrupt
 * 
 * @param context 
 */
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

/**
 * @brief Interrupt handler for when data is recieved
 * 
 * @param context 
 */
void UART::uart_irq_handler(void *context)
{
    UART *uart = static_cast<UART *>(context);

    while (uart_is_readable(uart->m_uart))
    {
        char c = uart_getc(uart->m_uart);
        uart->rx_buffer_.push(c);
    }
}

/**
 * @brief Parse the received message
 * 
 */
void UART::decode_message()
{
    char data[128]; // Allocate memory for the data buffer

    if (!read(data, sizeof(data)))
    {
        return;
    }

    printf("message: %s\n", data);

    uint8_t header = data[0];

    char response[128];

    response[0] = header;

    switch (header)
    {
        case message_headers::SET_ATTENUATION:
            set_attenuation(data);
            break;
        case message_headers::GET_ATTENUATION:
            get_attenuation(response);
            break;
        case message_headers::SET_LNA_ENABLE:
            set_lna_enable(data);
            break;
        case message_headers::GET_LNA_ENABLE:
            get_lna_enable(response);
            break;
        case message_headers::SET_ATTENUATOR_ENABLE:
            set_attenuator_enable(data);
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

    tx_buffer_.push(*response);
}

/**
 * @brief Send data in the tx buffer
 * 
 * @return size_t 
 */
size_t UART::send_message()
{
    size_t bytes_sent = 0;
    char data[128];
    while (!tx_buffer_.empty() && bytes_sent < sizeof(data))
    {
        data[bytes_sent++] = tx_buffer_.front();
        rx_buffer_.pop();
    }

    write(data);

    return bytes_sent > 0;
}

/**
 * @brief Set the attenuation level
 * 
 * @param data 
 * @return uint8_t 
 */
uint8_t UART::set_attenuation(char* data)
{
    uint8_t attenuation_value = data[1];
    uint8_t band_mask = data[2];

    return band_mask;
}

/**
 * @brief Get the current attenuation level
 * 
 * @param response 
 */
void UART::get_attenuation(const char* response)
{

}

/**
 * @brief Set the bands for which LNA is enabled/disabled
 * 
 * @param data 
 * @return uint8_t 
 */
uint8_t UART::set_lna_enable(char* data)
{
    uint8_t lna_enabled = data[1];
    uint8_t band_mask = data[2];

    return band_mask;
}

/**
 * @brief Get te bands on which LNA is enabled
 * 
 * @param response 
 */
void UART::get_lna_enable(const char* response)
{
    uint8_t lna_enabled;
}

/**
 * @brief Set the bands for which the attenuators should be enabled/disabled
 * 
 * @param data 
 * @return uint8_t 
 */
uint8_t UART::set_attenuator_enable(char* data)
{
    uint8_t attenuator_enabled = data[1];
    uint8_t band_mask = data[2];

    return band_mask;
}

/**
 * @brief Get the current attenuator enabled/disabled status for each band
 * 
 * @param response 
 */
void UART::get_attenuator_enable(const char* response)
{
    uint8_t attenuator_enabled;
}

/**
 * @brief Set the Calibration table on the EEPROM 
 * 
 * @param data 
 */
void UART::set_calibration(char* data)
{
    uint8_t calibration_table = data[1];
    uint8_t band_mask = data[2];
}

/**
 * @brief Get the current calibration table on the EEPROM
 * 
 * @param response 
 */
void UART::get_calibration(const char* response)
{
    // unknown return
}

/**
 * @brief A list of the attenuator stage bits
 * 
 * @param response 
 */
void UART::get_bits(const char* response)
{
    // unknown return
}

/**
 * @brief A list of the attenuator stage harder numbers
 * 
 * @param response 
 */
void UART::get_hardware_numbers(const char* response)
{
    // unknown return
}

/**
 * @brief A list of the attenuator stage software numbers
 * 
 * @param response 
 */
void UART::get_software_numbers(const char* response)
{
    // unknown return
}
