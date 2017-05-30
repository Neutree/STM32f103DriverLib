#include "USART.h"
#include "PN53x.h"
#include "TaskManager.h"
#include "LED.h"


USART LOG(1,115200,false,0,3,7,1,3);//
USART usart2(2,115200);//
USART usart3(3,115200);//
PN53x rfidReader(usart3);
GPIO gpioLed(GPIOC,13,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
LED led(gpioLed);

int main()
{
	led.On();
	TaskManager::DelayS(2);
	if(!rfidReader.PcdWakeUp())
		LOG<<"pcd wake up fail!!!\n";
	LOG<<"init complete\n";
	while(1)
	{
		uint16_t type;
		uint8_t uid[4];
		if(rfidReader.PcdListPassiveTarget(&type,uid))
		{
			LOG<<"card type:"<<type<<",uid:";
			LOG.PrintHex(uid,4,"-")<<"\n";
		}
		led.Blink(0,500,false);
	}
	
}

