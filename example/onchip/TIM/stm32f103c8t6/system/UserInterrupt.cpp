#include "UserInterrupt.h"

u8 cnt;
/*************************TIM_USER_IRQ***********************************/
#ifdef USE_TIMER1
void Timer1_IRQ()
{
	
}
#endif


#ifdef USE_TIMER2

void Timer2_IRQ()
{
	static bool flag = true;

	if(flag)
	{
		cnt++;
		if(cnt>100)
			flag=false;
	}	
	else
	{
		cnt--;
		if(cnt<1)
			flag=true;
	}
			
}
#endif

#ifdef USE_TIMER3
void Timer3_IRQ()
{
	
}
#endif

#ifdef USE_TIMER4
void Timer4_IRQ()
{
	
}
#endif

