#ifndef __INTERRUPT_H
#define __INTERRUPT_H

#include "UserInterrupt.h"
#include "Configuration.h"
#include "stm32f10x.h"


/*USART*************************************************************************/
#ifdef USE_USART1
extern USART *pUSART1;
#endif

#ifdef USE_USART2
extern USART *pUSART2;
#endif

#ifdef USE_USART3
extern USART *pUSART3;
#endif

/*END****************************************************************************/


/*CAN****************************************************************************/
#ifdef USE_CAN
extern CAN *pCAN;
#endif
/*CAN END************************************************************************/


/*IIC****************************************************************************/
#ifdef USE_I2C1
extern I2C *pI2C1;
#endif

#ifdef USE_I2C2
extern I2C *pI2C2;
#endif
/*END****************************************************************************/




/*Capture************************************************************************/
#ifdef USE_CAPTURE_TIM1
extern InputCapture_TIM *pICP1;
#endif

#ifdef USE_CAPTURE_TIM2
extern InputCapture_TIM *pICP2;
#endif

#ifdef USE_CAPTURE_TIM3
extern InputCapture_TIM *pICP3;
#endif

#ifdef USE_CAPTURE_TIM4
extern InputCapture_TIM *pICP4;
#endif
#ifdef USE_CAPTURE_EXIT0
extern InputCapture_EXIT *pICPExt0;
#endif
#ifdef USE_CAPTURE_EXIT1
extern InputCapture_EXIT *pICPExt1;
#endif
#ifdef USE_CAPTURE_EXIT2
extern InputCapture_EXIT *pICPExt2;
#endif
#ifdef USE_CAPTURE_EXIT3
extern InputCapture_EXIT *pICPExt3;
#endif
#ifdef USE_CAPTURE_EXIT4
extern InputCapture_EXIT *pICPExt4;
#endif
#ifdef USE_CAPTURE_EXIT5
extern InputCapture_EXIT *pICPExt5;
#endif
#ifdef USE_CAPTURE_EXIT6
extern InputCapture_EXIT *pICPExt6;
#endif
#ifdef USE_CAPTURE_EXIT7
extern InputCapture_EXIT *pICPExt7;
#endif
#ifdef USE_CAPTURE_EXIT8
extern InputCapture_EXIT *pICPExt8;
#endif
#ifdef USE_CAPTURE_EXIT9
extern InputCapture_EXIT *pICPExt9;
#endif
#ifdef USE_CAPTURE_EXIT10
extern InputCapture_EXIT *pICPExt10;
#endif
#ifdef USE_CAPTURE_EXIT11
extern InputCapture_EXIT *pICPExt11;
#endif
#ifdef USE_CAPTURE_EXIT12
extern InputCapture_EXIT *pICPExt12;
#endif
#ifdef USE_CAPTURE_EXIT13
extern InputCapture_EXIT *pICPExt13;
#endif
#ifdef USE_CAPTURE_EXIT14
extern InputCapture_EXIT *pICPExt14;
#endif
#ifdef USE_CAPTURE_EXIT15
extern InputCapture_EXIT *pICPExt15;
#endif

/*END****************************************************************************/






extern "C"{

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void I2C1_ER_IRQHandler(void);
void I2C2_EV_IRQHandler(void);
void I2C2_ER_IRQHandler(void);
void TIM1_UP_IRQHandler(void);
void TIM2_IRQHandler(void) ;
void TIM3_IRQHandler(void) ;
void TIM4_IRQHandler(void) ;
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void) ;
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void USB_LP_CAN1_RX0_IRQHandler(void);

//////////////////////////////////
///USART3 send channel 
//////////////////////////////////
void DMA1_Channel2_IRQHandler();
/////////////////////////////////
///USART1 DMA send channel
///I2C2   DMA send channel
/////////////////////////////////
void DMA1_Channel4_IRQHandler();
///////////////////////////////
///I2C2 DMA receive channel
//////////////////////////////
void DMA1_Channel5_IRQHandler();

///////////////////////////////
///I2C1   DMA send channel
//////////////////////////////
void DMA1_Channel6_IRQHandler();

/////////////////////////////////
///USART2 DMA send channel
///I2C1   DMA receive channel
////////////////////////////////
void DMA1_Channel7_IRQHandler();
	
}

#endif
