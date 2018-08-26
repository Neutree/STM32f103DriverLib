#include "USART.h"
#include "IAP_USART.h"
#include "flash.h"
#include "GPIO.h"
#include "stdint.h"
#include "LED_Min.h"

const uint32_t iapAddr = (0x08000000);
const uint32_t appAddr = (0x08000000+10*MEMORY_PAGE_SIZE);

USART usart(1,115200);
Flash flash(appAddr);
IAP_USART iapUsart(usart,flash,appAddr);

GPIO ledPin(GPIOC,13);
LED  led(ledPin);



int main()
{
	IAP_USART_Upgrade_Status_t status;
	
	iapUsart.Init(iapAddr);
	
	led.Blink(4,200,true);

	while(1)
	{
		status = iapUsart.Upgrade();
		switch((int)status)
		{
			case IAP_USART_UPGRADE_STATUS_IDLE:
				led.Blink(0,1000,false);
				break;
			case IAP_USART_UPGRADE_STATUS_WORKING:
				led.Blink(0,100,false);
				break;
			case IAP_USART_UPGRADE_STATUS_COMPLETE:
				iapUsart.LoadApp();
				led.Off();
				break;
			case IAP_USART_UPGRADE_STATUS_ERROR:
				led.On();
				break;
		}
	}
}

