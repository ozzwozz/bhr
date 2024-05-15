#pragma once

#include "I2CDevice.h"


/// @class PCA9554 Attenuator Driver
/// @brief Driver for the PCA9554
class PCA9554 : public I2CDevice
{
    public:
        /// @brief Construct a new PCA9554::PCA9554 object
        /// @param i2c 
        /// @param address 
        PCA9554(i2c_inst_t *i2c, uint8_t address);

        /// @brief Destroy the PCA9554::PCA9554 object
        ~PCA9554();

        /// @brief Write data to the attenuator cards
        /// @param value 
        void set_outputs(const uint8_t value);

        /// @brief Read data from the attenuator cards
        /// @return uint8_t 
        uint8_t read_inputs();
};
