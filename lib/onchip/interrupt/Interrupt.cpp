/**
  *@file Interrupt.h
  *@date 2015-10-27
  *@author 
  *@breif  interrupt management file
  *
  */

#include "Interrupt.h"

#ifdef USE_USART
USART *pUSART1 = 0;
USART *pUSART2 = 0;
USART *pUSART3 = 0;
void USART1_IRQHandler(void)
{
	if(pUSART1)
		pUSART1->Irq();
}
void USART2_IRQHandler(void)
{
	if(pUSART2)
		pUSART2->Irq();
}
void USART3_IRQHandler(void)
{
	if(pUSART3)
		pUSART3->Irq();
}
#endif

void DMA1_Channel4_IRQHandler()
{	
#ifdef USE_USART
	if(pUSART1)
		pUSART1->DmaIrq();
#endif
}

void DMA1_Channel7_IRQHandler()
{	
#ifdef USE_USART
	if(pUSART2)
		pUSART2->DmaIrq();
#endif
}


void DMA1_Channel2_IRQHandler()
{	
#ifdef USE_USART
	if(pUSART3)
		pUSART3->DmaIrq();
#endif
}




