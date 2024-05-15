#include "SPIDevice.h"


/// @brief Construct a new SPIDevice::SPIDevice object
/// @param spi_inst 
/// @param cs_pin 
SPIDevice::SPIDevice(spi_inst_t* spi_inst, uint cs_pin)
    : m_spi(spi_inst)
    , m_cs_pin(cs_pin)
    {
    // Initialize SPI communication
    spi_init(m_spi, 1000000);  // Default baudrate is 1MHz
    gpio_init(m_cs_pin);
    gpio_set_dir(m_cs_pin, GPIO_OUT);
    deassertCS(); // Set CS pin high (inactive)
}

/// @brief Destroy the SPIDevice::SPIDevice object
SPIDevice::~SPIDevice()
{

}

/// @brief Configure the SPI
/// @param baudrate 
void SPIDevice::configure(uint baudrate)
{
    spi_set_baudrate(m_spi, baudrate);
}

/// @brief Select the device
void SPIDevice::assertCS()
{
    gpio_put(m_cs_pin, 0);
}

/// @brief unselect the device
void SPIDevice::deassertCS()
{
    gpio_put(m_cs_pin, 1);
}

/// @brief write to the device
/// @param data 
/// @param len 
void SPIDevice::write(const uint8_t *data, size_t len)
{
    assertCS();
    spi_write_blocking(m_spi, data, len);
    deassertCS();
}


/// @brief read from the device
/// @param data 
/// @param len 
void SPIDevice::read(uint8_t *data, size_t len)
{
    assertCS();
    spi_read_blocking(m_spi, 0, data, len);
    deassertCS();
}


/// @brief Read and write to the target device
/// @param tx_data 
/// @param rx_data 
/// @param len 
void SPIDevice::exchange(const uint8_t *tx_data, uint8_t *rx_data, size_t len)
{
    assertCS();
    spi_write_read_blocking(m_spi, tx_data, rx_data, len);
    deassertCS();
}
