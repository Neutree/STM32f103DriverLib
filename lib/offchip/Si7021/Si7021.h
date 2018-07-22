#ifndef __SI7021_H
#define __SI7021_H

#include "Temperature.h"
#include "I2C.h"
#include "Humidity.h"


#define SI7021_CHIP_ID_SI7013      0x0D
#define SI7021_CHIP_ID_SI7020      0x14
#define SI7021_CHIP_ID_SI7021      0x15

class Si7021 :public Temperature,public Humidity{

public:
    Si7021();
    bool Init(I2C* i2c);
    virtual float GetTemperature();
    virtual float GetHumidity();
    virtual int Update();
    int8_t GetChipID();
	bool Kick();

private:
    I2C* mI2C;
    float mTemperature;
    float mHumidity;
};


#endif

