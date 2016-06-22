/**
  *@file Interrupt.h
  *@date 2015-10-27
  *@author 
  *@breif  interrupt management file
  *
  */

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#define USE_USART


#ifdef USE_USART
#include "USART.h"
extern USART *pUSART1;
extern USART *pUSART2;
extern USART *pUSART3;
#endif

extern "C"{
	
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
	
}

#endif
