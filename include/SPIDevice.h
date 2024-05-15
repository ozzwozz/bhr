#include "pico/stdlib.h"
#include "hardware/spi.h"

/**
 * @brief Super class for SPIDevices to inherit from
 * 
 */
class SPIDevice {
public:
    SPIDevice(spi_inst_t* spi_inst, uint cs_pin);
    ~SPIDevice();

    void configure(uint baudrate = 1000000); // Default baudrate is 1MHz

    void write(const uint8_t *data, size_t len);
    void read(uint8_t *data, size_t len);
    void exchange(const uint8_t *tx_data, uint8_t *rx_data, size_t len);

protected:
    /**
     * @param m_spi member spi instance 
     */
    spi_inst_t* m_spi;
    /**
     * @param m_cs_pin member chip select pin
     */
    uint m_cs_pin;

    void assertCS();
    void deassertCS();
};