#include "USART.h"
#include "TaskManager.h"
#include "GPIO.h"
#include "Temperature.h"
#include "Si7021.h"
#include "I2C.h"
#include "Assert.h"

USART log(3,115200);

I2C i2c1(1);
Si7021 tempHumi;



bool Init()
{
	//temperature and humidity
	tempHumi.Init(&i2c1);
	//chip id
	if(tempHumi.Kick())
	{
		log<<"find Si7021\n";
	}
	else
	{
		log<<"no Si7021 found\n";
		return false;
	}
	return true;
}

int main()
{
	log<<"system start, initilize now\n";

	Init();
	
	while(1)
	{

		//update
		tempHumi.Update();
	
		//temperature humidity
		log<<"temperature:"<<tempHumi.GetTemperature()<<"\n";
		log<<"humidity:"<<tempHumi.GetHumidity()<<"\n";
		TaskManager::DelayMs(100);
	}
}



