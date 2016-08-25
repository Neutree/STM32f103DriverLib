
#include "Interrupt.h"


/*USART*************************************************************************/
#ifdef USE_USART1
USART *pUSART1 = 0;
#endif

#ifdef USE_USART2
USART *pUSART2 = 0;
#endif

#ifdef USE_USART3
USART *pUSART3 = 0;
#endif

void USART1_IRQHandler(void)    //----USART1 IRQ----//
{
#ifdef USE_USART1
	pUSART1->Irq();
#endif
}
void USART2_IRQHandler(void)    //----USART2 IRQ----//
{
#ifdef USE_USART2
	pUSART2->Irq();
#endif
}
void USART3_IRQHandler(void)    //----USART3 IRQ----//
{
#ifdef USE_USART3
	pUSART3->Irq();
#endif
}




/*TIM****************************************************************************/
#ifdef USE_CAPTURE_TIM1
InputCapture_TIM *pICP1;
#endif

#ifdef USE_CAPTURE_TIM2
InputCapture_TIM *pICP2;
#endif

#ifdef USE_CAPTURE_TIM3
InputCapture_TIM *pICP3;
#endif

#ifdef USE_CAPTURE_TIM4
InputCapture_TIM *pICP4;
#endif

void TIM1_UP_IRQHandler(void)   //----TIM1 Up-------//
{ 
#ifdef USE_TIMER1
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
	Timer1_IRQ();
#endif
}

#ifdef USE_CAPTURE_TIM1
void TIM1_CC_IRQHandler(void)
{
	pICP1->IRQ();
}
#endif

void TIM2_IRQHandler(void)      //----TIM4 IRQ------// 
{
#ifdef USE_TIMER2
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
	Timer2_IRQ();
#endif	
	
#ifdef USE_CAPTURE_TIM2
	pICP2->IRQ();
#endif
}

void TIM3_IRQHandler(void)      //----TIM4 IRQ------// 
{
#ifdef USE_TIMER3
	TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
	Timer3_IRQ();
#endif	
	
#ifdef USE_CAPTURE_TIM3
	pICP3->IRQ();
#endif
}
void TIM4_IRQHandler(void)      //----TIM4 IRQ------// 
{
#ifdef USE_TIMER4
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
	Timer4_IRQ();
#endif	
	
#ifdef USE_CAPTURE_TIM4
	pICP4->IRQ();
#endif
}
/*END****************************************************************************/

