#include "USART.h"
#include "PN53x.h"
#include "TaskManager.h"
#include "LED.h"


USART LOG(1,115200,false,0,3,7,1,3);//
USART usart2(2,115200);//
USART usart3(3,115200);//
PN53x rfidReader(usart3);
GPIO gpioLed(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
LED led(gpioLed);

uint8_t key[6]={0xff,0xff,0xff,0xff,0xff,0xff};
uint8_t dataRead[16] = {0};
uint8_t dataWrite[16] = {0,0,0,0,5,6,7,8,9,0,0,0,0,0,0,0};

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
			LOG<<"find card,type:"<<type<<",uid:  ";
			LOG.PrintHex(uid,4,"-")<<"\n";
			if(rfidReader.PcdVerifyKeyA(uid,key))
			{
				LOG<<"auth success!\n";
				memcpy(dataWrite+12,uid,4);
				memcpy(dataWrite,uid,4);
				if(rfidReader.PcdWrite(dataWrite))
				{
					LOG<<"write success:";
					LOG.PrintHex(dataWrite,16," ")<<"\n";
					if(rfidReader.PcdRead(dataRead))
					{
						LOG<<"read success:";
						LOG.PrintHex(dataRead,16,",")<<"\n";
					}
				}
			}
		}
		led.Blink(0,500,false);
		TaskManager::DelayS(3);
	}
	
}

