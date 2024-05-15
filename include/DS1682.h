#include "I2CDevice.h"


 ///@file DS1682 - Total-Elapsed-Time Recorder with Alarm
 

class DS1682 : public I2CDevice {
public:
    DS1682(i2c_inst_t* i2c_inst, uint8_t address);
    ~DS1682();

    bool setTime(const uint32_t timestamp);
    uint32_t getTime();
    uint32_t getUniqueID();
};
