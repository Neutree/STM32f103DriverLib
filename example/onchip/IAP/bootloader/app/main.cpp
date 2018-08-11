#include "USART.h"
#include "IAP_USART.h"


USART usart(1,115200);
IAP_USART iapUsart(usart);


int main()
{
	IAP_USART_Upgrade_Status_t status;

	while(1)
	{
		status = iapUsart.Upgrade();
		if(status == IAP_USART_UPGRADE_STATUS_COMPLETE)
			iapUsart.LoadApp();
	}
}

