#include "I2CDevice.h"

/**
 * @file DS1682 - Total-Elapsed-Time Recorder with Alarm
 * 
 */

class DS1682 : public I2CDevice {
public:
    DS1682(i2c_inst_t* i2c_inst, uint8_t address);

    bool setTime(uint32_t timestamp);
    uint32_t getTime();
    uint32_t getUniqueID();

private:
    uint8_t readRegister(uint8_t regAddress);
    bool writeRegister(uint8_t regAddress, uint8_t data);
};
