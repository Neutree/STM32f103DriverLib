#include "LED.h"

GPIO ledPin(GPIOC,13);
LED led(ledPin);

int main()
{
	IAP_USART_Upgrade_Status_t status;

	while(1)
	{
		led.Blink(0,0.3,false);
	}
}

