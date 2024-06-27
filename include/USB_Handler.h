#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include <queue>

#include "PCA9554.h"
#include "MAX31725.h"
#include "M24M02.h"
#include "DS1682.h"
#include "adc.h"
#include "SI53361.h"


/// @class USB_Handler
/// @brief UART Driver Class
class USB_Handler
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
        SET_10MHZ_REFERENCE_SOURCE = 0x06,
        GET_10MHZ_REFERENCE_SOURCE = 0x07,
        SET_CALIBRATION = 0x10,
        GET_CALIBRATION = 0x11,
        GET_BITS = 0X20,
        GET_HARDWARE_NUMBERS = 0X21,
        GET_SOFTWARE_NUMBERS = 0X22,
        SET_HARDWARE_NUMBERS = 0X23,
        SET_SOFTWARE_NUMBERS = 0X24,
        EXTERNAL_INTERRUPT = 0x30,
        OVERTEMP_INTERRUPT = 0x31,
        RESET_ETC = 0x50,
        SET_P3V3_OXCO_PIN = 0x40,
        SET_P5V5_PIN = 0x41
    }; // Enum inside the class

    /// @brief Construct a new UART object
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
    USB_Handler(MAX31725 &max31725, M24M02 &m24m02, SI53361 &si53361
    , PCA9554 &pca9554_1, PCA9554 &pca9554_2, PCA9554 &pca9554_3, PCA9554 &pca9554_4, PCA9554 &pca9554_5
    , ADC &adc, DS1682 &ds1682);
            
    /// @brief Destroy the UART object
    ~USB_Handler();

    /// @brief Parse the received message
    void decode_message(const uint8_t message[5]);

    /// @brief get the external trigger flag state
    /// @return external trigger flag state
    bool get_ext_trig_flag();
    
    /// @brief get the overtemp flag state
    /// @return overtemp flag state
    bool get_overtemp_flag();

private:
    /// @param m_max31725 MAX31725 object
    MAX31725 &m_max31725;

    /// @param m_m24m02 M24M02 object
    M24M02 &m_m24m02;
    /// @param m_si53361 SI53361 object
    SI53361 &m_si53361;
    /// @param m_ds1682 DS1682 object
    DS1682 &m_ds1682;

    /// @param m_pca9554_1 PCA9554 object
    PCA9554 &m_pca9554_1;
    /// @param m_pca9554_2 PCA9554 object
    PCA9554 &m_pca9554_2;
    /// @param m_pca9554_3 PCA9554 object
    PCA9554 &m_pca9554_3;
    /// @param m_pca9554_4 PCA9554 object
    PCA9554 &m_pca9554_4;
    /// @param m_pca9554_5 PCA9554 object
    PCA9554 &m_pca9554_5;

    /// @param m_adc ADC object
    ADC &m_adc;

    /// @param m_ext_trig_pin External trigger pin
    static const uint m_ext_trig_pin = 15;

    /// @param m_overtemp_pin External temperture interrupt trigger pin
    static const uint m_overtemp_pin = 6;

    /// @param m_ext_trig_flag Flag set high when external trigger interrupt triggered
    volatile bool m_ext_trig_flag = false;

    /// @param m_overtemp_flag Flag set high when temperature interrupt is triggered
    volatile bool m_overtemp_flag = false;

    /// @brief Handler for the EXT_TRIG/GPIO15 interrupt
    /// @param gpio 
    /// @param events 
    static void gpio_callback(uint gpio, uint32_t event);//void* context);

    /// @brief Set the attenuation level
    /// @param response  
    /// @param data 
    void set_attenuation(uint8_t response[20], uint8_t data[5]);

    /// @brief Get the current attenuation level
    /// @param response 
    /// @param band_mask
    void get_attenuation(uint8_t response[20], uint8_t band_mask);

    /// @brief Set the bands for which LNA is enabled/disabled
    /// @param response 
    /// @param data  
    void set_lna_enable(uint8_t response[20], uint8_t data[5]);
    
    /// @brief Get te bands on which LNA is enabled
    /// @param response 
    void get_lna_enable(uint8_t response[20]);

    /// @brief Set the bands for which the attenuators should be enabled/disabled
    /// @param response  
    /// @param data 
    void set_attenuator_enable(uint8_t response[20], uint8_t data[5]);

    /// @brief Get the current attenuator enabled/disabled status for each band
    /// @param response 
    void get_attenuator_enable(uint8_t response[20]);

    /// @brief Set the clock to use
    /// @param response
    /// @param data the clock that should be used
    void set_clock_state(uint8_t response[20], uint8_t data[5]);

    /// @brief Get the current clock in use
    /// @param response 
    void get_clock_state(uint8_t response[20]);

    /// @brief Set the Calibration table on the EEPROM 
    /// @param data 
    void set_calibration(uint8_t data[5]);

    /// @brief Get the current calibration table on the EEPROM
    /// @param response 
    void get_calibration(uint8_t response[20]);

    /// @brief A list of the attenuator stage bits
    /// @param response 
    void get_bits(uint8_t response[20]);

    /// @brief A list of the attenuator stage harder numbers
    /// @param response 
    void get_hardware_numbers(uint8_t response[20]);

    /// @brief A list of the attenuator stage software numbers
    /// @param response 
    void get_software_numbers(uint8_t response[20]);
    
    /// @brief Set hardware numbers
    /// @param response 
    /// @param mutable_message 
    void set_hardware_numbers(uint8_t response[20], uint8_t mutable_message[5]);

    /// @brief Set software numbers
    /// @param response 
    /// @param mutable_message 
    void set_software_numbers(uint8_t response[20], uint8_t mutable_message[5]);

    /// @brief Call the ETC reset command
    void reset_etc();

    /// @brief Set the p3v3 OXCO pin
    /// @param mutable_message 
    void set_p3v3_oxco_pin(uint8_t mutable_message[5]);

    /// @brief Set the p5v5 pin
    /// @param mutable_message 
    void set_p5v5_pin(uint8_t mutable_message[3]);
};
