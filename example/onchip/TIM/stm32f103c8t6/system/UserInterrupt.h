#ifndef __USERINTERRUPT_H
#define __USERINTERRUPT_H
#include "Configuration.h"


extern u8 cnt;

/*TIM***************************************************************************/
#ifdef USE_TIMER1
void Timer1_IRQ();
#endif

#ifdef USE_TIMER2
 void Timer2_IRQ();
#endif

#ifdef USE_TIMER3
 void Timer3_IRQ();
#endif

#ifdef USE_TIMER4
 void Timer4_IRQ();
#endif

/*EDN***************************************************************************/



#endif

