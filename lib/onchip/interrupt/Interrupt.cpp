
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

/////////////////////////////////
///USART1 DMA send channel
///I2C2   DMA send channel
/////////////////////////////////
void DMA1_Channel4_IRQHandler()
{	
#ifdef USE_USART1
	if(pUSART1)
		pUSART1->DmaIrq();
#endif
#ifdef USE_I2C
#ifdef USE_I2C2
#ifdef I2C_USE_DMA
	if(pI2C2)
		pI2C2->DmaTxIRQ();
#endif
#endif
#endif
}

/////////////////////////////////
///USART2 DMA send channel
///I2C1   DMA receive channel
////////////////////////////////
void DMA1_Channel7_IRQHandler()
{	
#ifdef USE_USART2
	if(pUSART2)
		pUSART2->DmaIrq();
#endif

#ifdef USE_I2C
#ifdef USE_I2C1
#ifdef I2C_USE_DMA
	if(pI2C1)
		pI2C1->DmaRxIRQ();
#endif
#endif
#endif
}


void DMA1_Channel2_IRQHandler()
{	
#ifdef USE_USART3
	if(pUSART3)
		pUSART3->DmaIrq();
#endif
}
/*END****************************************************************************/


/*IIC****************************************************************************/
#ifdef USE_I2C1
I2C *pI2C1;
#endif

#ifdef USE_I2C2
I2C *pI2C2;
#endif

void I2C1_EV_IRQHandler(void)   //----I2C1 EVENT----//
{
#ifdef USE_I2C1
	pI2C1->EventIRQ();
#endif
}
void I2C1_ER_IRQHandler(void)   //----I2C1 ERROE----//
{
#ifdef USE_I2C1
	pI2C1->ErrorIRQ();
#endif
}
void I2C2_EV_IRQHandler(void)   //----I2C2 EVENT----//
{
#ifdef USE_I2C2
	pI2C2->EventIRQ();
#endif
}
void I2C2_ER_IRQHandler(void)   //----I2C2 ERROE----//
{
#ifdef USE_I2C2
	pI2C2->ErrorIRQ();
#endif
}
///////////////////////////////
///I2C1   DMA send channel
//////////////////////////////
void DMA1_Channel6_IRQHandler()
{
#ifdef USE_I2C
#ifdef USE_I2C1
#ifdef I2C_USE_DMA
	if(pI2C1)
		pI2C1->DmaTxIRQ();
#endif
#endif
#endif
}

///////////////////////////////
///I2C2 DMA receive channel
//////////////////////////////
void DMA1_Channel5_IRQHandler()
{
	#ifdef USE_I2C
	#ifdef USE_I2C2
	#ifdef I2C_USE_DMA
		if(pI2C2)
			pI2C2->DmaRxIRQ();
	#endif
	#endif
	#endif
}

/*END****************************************************************************/



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


/*EXIT***************************************************************************/
void EXTI0_IRQHandler(void) 	  //----EXTI0 IRQ-----//
{
	EXTI_ClearITPendingBit(EXTI_Line0);
#ifdef USE_EXTI0
	EXTI0_IRQ();
#endif
	
#ifdef USE_CAPTURE_EXIT0
	pICPExt0->IRQ();
#endif
}
void EXTI1_IRQHandler(void) 	  //----EXTI1 IRQ-----//
{
	EXTI_ClearITPendingBit(EXTI_Line1);
#ifdef USE_EXTI1
	EXTI1_IRQ();
#endif

#ifdef USE_CAPTURE_EXIT1
	pICPExt1->IRQ();
#endif
}

void EXTI2_IRQHandler(void) 	  //----EXTI2 IRQ-----//
{
	EXTI_ClearITPendingBit(EXTI_Line2);
#ifdef USE_EXTI2
	EXTI2_IRQ();
#endif

#ifdef USE_CAPTURE_EXIT2
	pICPExt2->IRQ();
#endif
}

void EXTI3_IRQHandler(void) 	  //----EXTI3 IRQ-----//
{
	EXTI_ClearITPendingBit(EXTI_Line3);
#ifdef USE_EXTI3
	EXTI3_IRQ();
#endif

#ifdef USE_CAPTURE_EXIT3
	pICPExt3->IRQ();
#endif
}

void EXTI4_IRQHandler(void) 	  //----EXTI4 IRQ-----//
{
	EXTI_ClearITPendingBit(EXTI_Line4);
#ifdef USE_EXTI4
	EXTI4_IRQ();
#endif
	
#ifdef USE_CAPTURE_EXIT4
	pICPExt4->IRQ();
#endif
}

void EXTI9_5_IRQHandler(void) 	//---EXTI9_5 IRQ----//
{
	if(EXTI_GetITStatus(EXTI_Line5))       //--EXTI5--//
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
	#ifdef USE_EXTI5
		EXTI5_IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT5
		pICPExt5->IRQ();
	#endif
	}
	if(EXTI_GetITStatus(EXTI_Line6))       //--EXTI6--//
	{
		EXTI_ClearITPendingBit(EXTI_Line6);
	#ifdef USE_EXTI6
		EXTI6_IRQ();
	#endif
			
	#ifdef USE_CAPTURE_EXIT6
		pICPExt6->IRQ();
					
	#endif
	}	
	if(EXTI_GetITStatus(EXTI_Line7))       //--EXTI7--//
	{
		EXTI_ClearITPendingBit(EXTI_Line7);
	#ifdef USE_EXTI7
		EXTI7_IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT7
		pICPExt7->IRQ();
	#endif
	}	
	if(EXTI_GetITStatus(EXTI_Line8))       //--EXTI8--//
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	#ifdef USE_EXTI8
		EXTI8_IRQ();
	#endif
			
	#ifdef USE_CAPTURE_EXIT8
		pICPExt8->IRQ();
	#endif
	}
	if(EXTI_GetITStatus(EXTI_Line9))       //--EXTI9--//
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	#ifdef USE_EXTI9
		EXTI9_IRQ();
	#endif
				
	#ifdef USE_CAPTURE_EXIT9
		pICPExt9->IRQ();
	#endif
	}
}

void EXTI15_10_IRQHandler(void) //---EXTI15_10 IRQ--//
{
	if(EXTI_GetITStatus(EXTI_Line10))     //--EXTI10--//
	{
		EXTI_ClearITPendingBit(EXTI_Line10); 
	#ifdef USE_EXTI10
		EXTI10_IRQ();
	#endif
		
	#ifdef USE_ULTRASONIC_EXTI10
		pUltExt10->IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT10
	pICPExt10->IRQ();
	#endif
	}
	
	if(EXTI_GetITStatus(EXTI_Line11))     //--EXTI11--//
	{
		EXTI_ClearITPendingBit(EXTI_Line11);
	#ifdef USE_EXTI11
		EXTI11_IRQ();
	#endif
		
	#ifdef USE_ULTRASONIC_EXTI11
		pUltExt11->IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT11
		pICPExt11->IRQ();
	#endif
	}
	
	if(EXTI_GetITStatus(EXTI_Line12))     //--EXTI12--//
	{
		EXTI_ClearITPendingBit(EXTI_Line12);
	#ifdef USE_EXTI12
		EXTI12_IRQ();
	#endif
		
	#ifdef USE_ULTRASONIC_EXTI12
		pUltExt12->IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT12
		pICPExt12->IRQ();
	#endif
	}
	
	if(EXTI_GetITStatus(EXTI_Line13))     //--EXTI13--//
	{
		EXTI_ClearITPendingBit(EXTI_Line13);
	#ifdef USE_EXTI13
		EXTI13_IRQ();
	#endif
		
	#ifdef USE_ULTRASONIC_EXTI13
		pUltExt13->IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT13
		pICPExt13->IRQ();
	#endif
	}
	
	if(EXTI_GetITStatus(EXTI_Line14))     //--EXTI14--//
	{
		EXTI_ClearITPendingBit(EXTI_Line14);
	#ifdef USE_EXTI14
		EXTI14_IRQ();
	#endif
		
	#ifdef USE_ULTRASONIC_EXTI14
		pUltExt14->IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT14
		pICPExt14->IRQ();
	#endif
	}
	
	if(EXTI_GetITStatus(EXTI_Line15))     //--EXTI15--//
	{
		EXTI_ClearITPendingBit(EXTI_Line15);
	#ifdef USE_EXTI15
		EXTI15_IRQ();
	#endif
		
	#ifdef USE_ULTRASONIC_EXTI15
		pUltExt15->IRQ();
	#endif
		
	#ifdef USE_CAPTURE_EXIT15
		pICPExt15->IRQ();
	#endif
	}
}

/*END****************************************************************************/


/*Capture*************************************************************************/

#ifdef USE_CAPTURE_EXIT0
InputCapture_EXIT *pICPExt0;
#endif
#ifdef USE_CAPTURE_EXIT1
InputCapture_EXIT *pICPExt1;
#endif
#ifdef USE_CAPTURE_EXIT2
InputCapture_EXIT *pICPExt2;
#endif
#ifdef USE_CAPTURE_EXIT3
InputCapture_EXIT *pICPExt3;
#endif
#ifdef USE_CAPTURE_EXIT4
InputCapture_EXIT *pICPExt4;
#endif
#ifdef USE_CAPTURE_EXIT5
InputCapture_EXIT *pICPExt5;
#endif
#ifdef USE_CAPTURE_EXIT6
InputCapture_EXIT *pICPExt6;
#endif
#ifdef USE_CAPTURE_EXIT7
InputCapture_EXIT *pICPExt7;
#endif
#ifdef USE_CAPTURE_EXIT8
InputCapture_EXIT *pICPExt8;
#endif
#ifdef USE_CAPTURE_EXIT9
InputCapture_EXIT *pICPExt9;
#endif
#ifdef USE_CAPTURE_EXIT10
InputCapture_EXIT *pICPExt10;
#endif
#ifdef USE_CAPTURE_EXIT11
InputCapture_EXIT *pICPExt11;
#endif
#ifdef USE_CAPTURE_EXIT12
InputCapture_EXIT *pICPExt12;
#endif
#ifdef USE_CAPTURE_EXIT13
InputCapture_EXIT *pICPExt13;
#endif
#ifdef USE_CAPTURE_EXIT14
InputCapture_EXIT *pICPExt14;
#endif
#ifdef USE_CAPTURE_EXIT15
InputCapture_EXIT *pICPExt15;
#endif


/*END****************************************************************************/

