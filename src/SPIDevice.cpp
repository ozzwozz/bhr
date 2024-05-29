#include "SPIDevice.h"

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

SPIDevice::~SPIDevice()
{

}

void SPIDevice::configure(uint baudrate)
{
    spi_set_baudrate(m_spi, baudrate);
}

void SPIDevice::assertCS()
{
    gpio_put(m_cs_pin, 0);
}

void SPIDevice::deassertCS()
{
    gpio_put(m_cs_pin, 1);
}

void SPIDevice::write(const uint8_t *data, size_t len)
{
    // select the device
    assertCS();
    if (spi_is_writable(m_spi))
    {
        spi_write_blocking(m_spi, data, len);
    }
    // deselect the device
    deassertCS();
}

void SPIDevice::read(uint8_t *data, size_t len)
{
    // select the device
    assertCS();
    if (spi_is_readable(m_spi))
    {
        spi_read_blocking(m_spi, 0, data, len);
    }
    // deselect the device
    deassertCS();
}

void SPIDevice::exchange(const uint8_t *tx_data, uint8_t *rx_data, size_t len)
{
    // select the device
    assertCS();
    spi_write_read_blocking(m_spi, tx_data, rx_data, len);
    // deselect the device
    deassertCS();
}
