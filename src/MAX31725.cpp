#include "MAX31725.h"

MAX31725::MAX31725(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
    set_over_temp_limit();
}

MAX31725::~MAX31725()
{
    uint8_t shutdown_cmd[2] { 0x01 , 0x01};

    write(shutdown_cmd, 2);
}

bool MAX31725::set_over_temp_limit()
{
    uint8_t tx_data[2] {m_cmd_set_temp_limit, m_temperature_limit_C};

    if (!write(tx_data, 1))
    {
        return false;
    }

    return true;
}

bool MAX31725::read_temperature(float &temperature)
{
    uint8_t rx_data[2] {m_cmd_read_temperature, 0x80};

    // using a special i2c_write because we want to keep master control
    int ret = i2c_write_blocking(m_i2c, m_address, 0x00, 1, true);
    if (ret == PICO_ERROR_GENERIC)
    {
        return false;
    }

    if (read(rx_data, 2) == PICO_ERROR_GENERIC)
    {
        return false;
    }

    temperature = rx_data[0] + (rx_data[1] / 256.0);

    return true;
}
