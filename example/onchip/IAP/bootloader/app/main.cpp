#include "USART.h"
#include "IAP_USART.h"
#include "flash.h"
#include "GPIO.h"

const uint32_t appAddr = (0x08000000+54*MEMORY_PAGE_SIZE);

USART usart(1,115200);
Flash flash(appAddr);
IAP_USART iapUsart(usart,flash,appAddr);

GPIO led(GPIOC,13);



int main()
{
	IAP_USART_Upgrade_Status_t status;
	
	led.SetLevel(1);

	while(1)
	{
		status = iapUsart.Upgrade();
		if(status == IAP_USART_UPGRADE_STATUS_COMPLETE)
		{
			//iapUsart.LoadApp();
			led.SetLevel(0);
		}
	}
}

