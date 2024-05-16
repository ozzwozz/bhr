#include "MAX31725.h"

MAX31725::MAX31725(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

MAX31725::~MAX31725()
{
}

void MAX31725::read_temperature(float &temperature)
{
    uint8_t rx_data[2];

    write(&cmd_read_temperature, 1);

    if (read(rx_data, 2))
    {
        int16_t temp_data = (rx_data[0] << 8) | rx_data[1];
        temperature = (temp_data / 256.0);
    }
}
