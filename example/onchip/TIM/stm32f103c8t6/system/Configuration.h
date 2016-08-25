#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "stm32f10x.h"
#include "USART.h"


//~~~~~~~~~~~~~~~USART Congfigure~~~~~~~~~~~//
#define USE_USART              //---USART---//
#define USE_USART1             //---USART1--//
//#define USE_USART2           //---USART2--//
//#define USE_USART3           //---USART3--//

//~~~~~~~~~~~~~~~Timer Congfigure~~~~~~~~~~~//
#define USE_TIMER              //---Timer---//
//#define USE_TIMER1              //---Timer1--//
#define USE_TIMER2             //---Timer2--//
//#define USE_TIMER3            //---Timer3--//
//#define USE_TIMER4            //---Timer4--//




#ifdef USE_TIMER
#include "Timer.h"
#endif

#ifdef USE_USART
#include "USART.h"
#endif

#endif

