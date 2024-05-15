#include "PCA9554.h"


 /// @brief Construct a new PCA9554::PCA9554 object
 
 /// @param i2c 
 /// @param address 
 
PCA9554::PCA9554(i2c_inst_t *i2c, uint8_t address) : I2CDevice(i2c, address)
{
}


 /// @brief Destroy the PCA9554::PCA9554 object
 
PCA9554::~PCA9554()
{
}


 /// @brief Write data to the attenuator cards
 
 /// @param value 
 
void PCA9554::set_outputs(const uint8_t value)
{
    uint8_t current_value = 0;
    I2CDevice::read(&current_value, 1);

    uint8_t new_value = (current_value & 0xFF) | (value & 0xFF);

    // TODO: probably not 16
    if(!I2CDevice::write(&new_value, 1))
    {
        ; // Pass because I don't know what I wanted here
    }
}


 /// @brief Read data from the attenuator cards
 
 /// @return uint8_t 
 
uint8_t PCA9554::read_inputs()
{
    uint8_t data;
    I2CDevice::read(&data, 1);
    return data;
}
