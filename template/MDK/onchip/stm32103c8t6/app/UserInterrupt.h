#ifndef __USERINTERRUPT_H
#define __USERINTERRUPT_H
#include "Configuration.h"


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

/*EXIT***************************************************************************/
#ifdef USE_EXTI0
extern void	EXTI0_IRQ();
#endif
#ifdef USE_EXTI1
extern void	EXTI1_IRQ();
#endif
#ifdef USE_EXTI2
extern void	EXTI2_IRQ();
#endif
#ifdef USE_EXTI3
extern void	EXTI3_IRQ();
#endif
#ifdef USE_EXTI4
extern void	EXTI4_IRQ();
#endif
#ifdef USE_EXTI5
extern void	EXTI5_IRQ();
#endif
#ifdef USE_EXTI6
extern void	EXTI6_IRQ();
#endif
#ifdef USE_EXTI7
extern void	EXTI7_IRQ();
#endif
#ifdef USE_EXTI8
extern void	EXTI8_IRQ();
#endif
#ifdef USE_EXTI9
extern void	EXTI9_IRQ();
#endif
#ifdef USE_EXTI10
extern void	EXTI10_IRQ();
#endif
#ifdef USE_EXTI11
extern void	EXTI11_IRQ();
#endif
#ifdef USE_EXTI12
extern void	EXTI12_IRQ();
#endif
#ifdef USE_EXTI13
extern void	EXTI13_IRQ();
#endif
#ifdef USE_EXTI14
extern void	EXTI14_IRQ();
#endif
#ifdef USE_EXTI15
extern void	EXTI15_IRQ();
#endif
/*END****************************************************************************/


#endif

