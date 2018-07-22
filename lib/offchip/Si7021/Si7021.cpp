
#include "Si7021.h"
#include "Assert.h"
#include "stdio.h"


#define SI7021_ADDR         0x40

// I2C commands
#define SI7021_RH_READ             0xE5 
#define SI7021_TEMP_READ           0xE3 
#define SI7021_RH_READ_NO_HOLD     0xF5 
#define SI7021_TEMP_READ_NO_HOLD   0xF3 
#define SI7021_POST_RH_TEMP_READ   0xE0 
#define SI7021_RESET               0xFE 
#define SI7021_USER1_READ          0xE7 
#define SI7021_USER1_WRITE         0xE6 



Si7021::Si7021()
{
    
}


bool Si7021::Init(I2C* i2c)
{
    assert(i2c != NULL);
    mI2C = i2c;
    return mI2C->Init();
}



float Si7021::GetTemperature()
{
    assert(mI2C != NULL);
    return mTemperature;
}

int8_t Si7021::GetChipID()
{
    //read chip id
	// compound commands
//  uint8_t SI7021_SERIAL1_READ[2]      ={ 0xFA, 0x0F };
	uint8_t SI7021_SERIAL2_READ[2]      ={ 0xFC, 0xC9 };
    uint8_t temp[6];
    if(!mI2C->AddCommandRead(SI7021_ADDR,SI7021_SERIAL2_READ,2,temp,6))
        return -1;
    uint8_t ret = mI2C->StartCMDQueue();
    if(ret != 1)
        return -2;
    if(!mI2C->WaitTransmitComplete(true,true,false))
        return -3;
    return temp[0];
}

bool Si7021::Kick()
{
	int8_t id = GetChipID();
	if(id != SI7021_CHIP_ID_SI7021)
		return false;
	return true;
}


float Si7021::GetHumidity()
{
    assert(mI2C != NULL);
    return mHumidity;
}


int Si7021::Update()
{
    assert(mI2C != NULL);

    uint8_t cmd = SI7021_TEMP_READ;
    uint8_t temp[2];
    uint16_t tempInteger;
    uint8_t ret ;

    //read temperature
    if(!mI2C->AddCommandRead(SI7021_ADDR,&cmd,1,temp,2))
        return -1;
    ret = mI2C->StartCMDQueue();
    if(ret != 1)
        return -2;
    if(!mI2C->WaitTransmitComplete(true,true,false))
        return -3;
    tempInteger = (uint16_t)temp[0] << 8 | temp[1];
    mTemperature = (((17572 * tempInteger) >> 16) - 4685)/100.0;

    //read humidity
    cmd = SI7021_RH_READ;
    if(!mI2C->AddCommandRead(SI7021_ADDR,&cmd,1,temp,2))
        return -1;
    ret = mI2C->StartCMDQueue();
    if(ret != 1)
        return -2;
    if(!mI2C->WaitTransmitComplete(true,true,false))
        return -3;
    tempInteger = (uint16_t)temp[0] << 8 | temp[1];
    mHumidity = ((125* tempInteger) >> 16) - 6;

    return 0;
}

