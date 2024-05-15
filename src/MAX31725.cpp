#include "MAX31725.h"

/// @brief Construct a new MAX31725::MAX31725 object
/// @param i2c 
/// @param address  
MAX31725::MAX31725(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}

/// @brief Destroy the MAX31725::MAX31725 object
MAX31725::~MAX31725()
{
}

/// @brief Read register storing the temperature data on the MAX31725
/// @return float 
float MAX31725::read_temperature()
{
    float temperature;
    uint8_t rx_data[2];

    uint8_t cmd = 0x00;

    write(&cmd, 1);

    if (!read(rx_data, 2))
    {
        temperature = NULL;
    }

    int16_t temp_data = (rx_data[0] << 8) | rx_data[1];
    temperature = (temp_data / 256.0);

    return temperature;
}
