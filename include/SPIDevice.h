#include "pico/stdlib.h"
#include "hardware/spi.h"


/// @class SPIDevice
/// @brief Super class for SPIDevices to inherit from
class SPIDevice {
public:
    /// @brief Construct a new SPIDevice object
    /// @param spi_inst 
    /// @param cs_pin 
    SPIDevice(spi_inst_t* spi_inst, uint cs_pin);

    /// @brief Destroy the SPIDevice object
    virtual ~SPIDevice();

    /// @brief Configure the SPI
    /// @param baudrate 
    void configure(uint baudrate = 1000000); // Default baudrate is 1MHz

    /// @brief write to the device
    /// @param data 
    /// @param len 
    void write(const uint8_t *data, size_t len);

    /// @brief read from the device
    /// @param data 
    /// @param len 
    void read(uint8_t *data, size_t len);

    /// @brief Read and write to the target device
    /// @param tx_data 
    /// @param rx_data 
    /// @param len 
    void exchange(const uint8_t *tx_data, uint8_t *rx_data, size_t len);

protected:
    /// @param m_spi member spi instance 
    spi_inst_t* m_spi;
    /// @param m_cs_pin member chip select pin 
    uint m_cs_pin;

    /// @brief Select the device
    void assertCS();

    /// @brief unselect the device
    void deassertCS();
};
