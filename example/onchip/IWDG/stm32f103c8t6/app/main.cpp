#include "USART.h"
#include "IWDG.h"

USART usart(1,115200,false,0,3,7,1,3);//
IWDG_ iwdg(2);//2s

int main()
{
	usart<<"init complete\n";
	while(1)
	{
		iwdg.Feed();
		usart<<".";
	}
	
}

