#include "LED.h"
#include "USART.h"
#include "TaskManager.h"
#include "IAP.h"


const uint32_t appAddr = (0x08000000+10*MEMORY_PAGE_SIZE);


GPIO ledPin(GPIOC,13);
LED led(ledPin);
USART usart(1,115200);
IAP iap(NULL,appAddr);

int main()
{
	double logTimeRecord;
	int count = 0;
	
	iap.Init(appAddr);
	
	usart<<"App start...\n";
	TaskManager::DelayMs(1000);
	while(1)
	{
		led.Blink(0,2000,false);
		if(TaskManager::TimeSlice(logTimeRecord,1))
		{
			usart<<"hello "<<(++count)<<"\n";
		}
	}
}

