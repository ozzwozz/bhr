#pragma once

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <queue>

#include "PCA9554.h"
#include "MAX31725.h"
#include "M24M02.h"
#include "DS1682.h"
#include "adc.h"
#include "SI53361.h"


/// @class UART
/// @brief UART Driver Class
class UART
{
    public:
        // TODO: Issue #7 - Implement Payload Definitions
        enum message_headers
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
            GET_SOFTWARE_NUMBERS = 0X22,
            EXTERNAL_INTERRUPT = 0x30
        }; // Enum inside the class

        /// @brief Construct a new UART object
        /// @param uart 
        /// @param baud_rate 
        /// @param rx_pin 
        /// @param tx_pin
        /// @param max31725 Instantiated MAX31725 driver object
        /// @param m24m02 Instantiated M24M02 driver object
        /// @param si53361 Instantiated SI53361 driver object
        /// @param pca9554_1 Instantiated PCA9554 driver object
        /// @param pca9554_2 Instantiated PCA9554 driver object
        /// @param pca9554_3 Instantiated PCA9554 driver object
        /// @param pca9554_4 Instantiated PCA9554 driver object
        /// @param pca9554_5 Instantiated PCA9554 driver object
        /// @param adc Instantiated ADC driver object
        /// @param ds1682 Instantiated driver object
        UART(uart_inst_t *uart, uint baud_rate, uint rx_pin, uint tx_pin, MAX31725 &max31725
        , M24M02 &m24m02, SI53361 &si53361, PCA9554 &pca9554_1, PCA9554 &pca9554_2,
        PCA9554 &pca9554_3, PCA9554 &pca9554_4, PCA9554 &pca9554_5, ADC &adc, DS1682 &ds1682);

        /// @brief Destroy the UART object
        ~UART();

        /// @brief Write to UART
        /// @param data 
        void write(const char *data);

        /// @brief Write to UART
        /// @param data 
        /// @param len 
        void write(const char *data, size_t len);

        /// @brief Read from UART
        /// @param data 
        /// @param len 
        /// @return size_t 
        size_t read(char *data, size_t len);

        /// @brief Check if UART is available to interact with
        /// @return int 
        int available();

        /// @brief flush the Rx buffer
        void flush_rx();

        /// @brief flush the Tx buffer
        void flush_tx();

        /// @brief Send data in the tx buffer
        /// @return size_t 
        size_t send_message();

    private:
        /// @param m_uart uart instance
        uart_inst_t *m_uart;

        /// @param m_max31725 MAX31725 object
        MAX31725 m_max31725;

        /// @param m_m24m02 M24M02 object
        M24M02 m_m24m02;
        /// @param m_si53361 SI53361 object
        SI53361 m_si53361;
        /// @param m_ds1682 DS1682 object
        DS1682 m_ds1682;

        /// @param m_pca9554_1 PCA9554 object
        PCA9554 m_pca9554_1;
        /// @param m_pca9554_2 PCA9554 object
        PCA9554 m_pca9554_2;
        /// @param m_pca9554_3 PCA9554 object
        PCA9554 m_pca9554_3;
        /// @param m_pca9554_4 PCA9554 object
        PCA9554 m_pca9554_4;
        /// @param m_pca9554_5 PCA9554 object
        PCA9554 m_pca9554_5;

        /// @param m_adc ADC object
        ADC m_adc;

        /// @param m_ext_trig_pin External trigger pin
        uint m_ext_trig_pin;
        
        /// @param m_rx_pin rx pin
        uint m_rx_pin;
        /// @param m_tx_pin tx pin
        uint m_tx_pin;
        /// @param rx_buffer_ rx buffer queue
        std::queue<char> rx_buffer_;
        /// @param tx_buffer_ tx buffer queue
        std::queue<std::vector<char>> tx_buffer_;

        /// @brief Handler for the EXT_TRIG/GPIO15 interrupt
        /// @param context 
        static void ext_trig_irq_handler(void *context);

        /// @brief Interrupt handler for when data is recieved
        /// @param context 
        static void uart_irq_handler(void *context);

        /// @brief Parse the received message
        void decode_message();

        /// @brief Set the attenuation level
        /// @param data 
        /// @return uint8_t 
        uint8_t set_attenuation(char* data);

        /// @brief Get the current attenuation level
        /// @param response 
        void get_attenuation(std::vector<char>& response);

        /// @brief Set the bands for which LNA is enabled/disabled
        /// @param data 
        /// @return uint8_t 
        uint8_t set_lna_enable(char* data);
        
        /// @brief Get te bands on which LNA is enabled
        /// @param response 
        void get_lna_enable(std::vector<char>& response);

        /// @brief Set the bands for which the attenuators should be enabled/disabled
        /// @param data 
        /// @return uint8_t 
        uint8_t set_attenuator_enable(char* data);

        /// @brief Get the current attenuator enabled/disabled status for each band
        /// @param response 
        void get_attenuator_enable(std::vector<char>& response);

        /// @brief Set the Calibration table on the EEPROM 
        /// @param data 
        void set_calibration(char* data);

        /// @brief Get the current calibration table on the EEPROM
        /// @param response 
        void get_calibration(std::vector<char>& response);

        /// @brief A list of the attenuator stage bits
        /// @param response 
        void get_bits(std::vector<char>& response);

        /// @brief A list of the attenuator stage harder numbers
        /// @param response 
        void get_hardware_numbers(std::vector<char>& response);

        /// @brief A list of the attenuator stage software numbers
        /// @param response 
        void get_software_numbers(std::vector<char>& response);
};
