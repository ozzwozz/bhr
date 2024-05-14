#include "pico/stdlib.h"
#include "hardware/spi.h"

class SPIDevice {
public:
    SPIDevice(spi_inst_t* spi_inst, uint cs_pin);

    void configure(uint baudrate = 1000000); // Default baudrate is 1MHz

    void write(const uint8_t *data, size_t len);
    void read(uint8_t *data, size_t len);
    void exchange(const uint8_t *tx_data, uint8_t *rx_data, size_t len);

protected:
    spi_inst_t* m_spi;
    uint m_cs_pin;

    void assertCS();
    void deassertCS();
};