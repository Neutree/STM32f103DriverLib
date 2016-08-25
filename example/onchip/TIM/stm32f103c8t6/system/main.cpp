#include "USART.h"
#include "Timer.h"
#include "Delay.h"
#include "UserInterrupt.h"

USART usart(1,115200,false);
Timer tim(TIM2,1,0,0);

int main()
{
		tim.OnOrOffIrq(true);
	  tim.Start();
	
		while(1)
		{
			usart<<cnt<<"\n";
			Delay::Ms(100);
		}
}

