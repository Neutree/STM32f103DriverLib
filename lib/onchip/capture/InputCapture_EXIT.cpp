#include "InputCapture_EXIT.h"
#include "Interrupt.h"
#include "TaskManager.h"

InputCapture_EXIT::InputCapture_EXIT(GPIO_TypeDef *exitPort, u8 exitPin, u8 itGroup,u8 prePriority,u8 subPriority)
{
	mExitPort = exitPort;
	mExitPin = u16(1<<exitPin);
	
	//Stpe 1: GPIO Initialize: RCC & GPIO Pin
	u8 portSource;
	if(mExitPort==GPIOA)	     { RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); portSource = 0;}
	else if(mExitPort==GPIOB)  { RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); portSource = 1;}
	else if(mExitPort==GPIOC)	 { RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); portSource = 2;}
	else if(mExitPort==GPIOD)	 { RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE); portSource = 3;}
	else if(mExitPort==GPIOE)	 { RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE); portSource = 4;}
	else if(mExitPort==GPIOF)	 { RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE); portSource = 5;}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = mExitPin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(mExitPort,&GPIO_InitStructure);
	GPIO_ResetBits(mExitPort,mExitPin);
	GPIO_EXTILineConfig(portSource,exitPin);
	
  //Step 2: NVIC Initialize
	u32 priorityGroup[5] = {NVIC_PriorityGroup_0,NVIC_PriorityGroup_1,NVIC_PriorityGroup_2,NVIC_PriorityGroup_3,NVIC_PriorityGroup_4};
	u8 EXTIx_IRQn;			//?????	
	switch (exitPin)
	{
		case 0:
			EXTIx_IRQn = EXTI0_IRQn; 
			#ifdef USE_CAPTURE_EXIT0
				pICPExt0 = this;
			#endif
		break;
		case 1:
			EXTIx_IRQn = EXTI1_IRQn; 
			#ifdef USE_CAPTURE_EXIT1
				pICPExt1 = this;
			#endif
		break;
		case 2:
			EXTIx_IRQn = EXTI2_IRQn; 
			#ifdef USE_CAPTURE_EXIT2
				pICPExt2 = this;
			#endif
		break;
		case 3:
			EXTIx_IRQn = EXTI3_IRQn; 
			#ifdef USE_CAPTURE_EXIT3
				pICPExt3 = this;
			#endif
		break;
		case 4:
			EXTIx_IRQn = EXTI4_IRQn; 
			#ifdef USE_CAPTURE_EXIT4
				pICPExt4 = this;
			#endif
		break;
		case 5:
			#ifdef USE_CAPTURE_EXIT5
				pICPExt5 = this;
			#endif
			EXTIx_IRQn = EXTI9_5_IRQn; 
		break;
		case 6:
			#ifdef USE_CAPTURE_EXIT6
				pICPExt6 = this;
			  USART_SendData(USART1,'W');
			#endif
			EXTIx_IRQn = EXTI9_5_IRQn; 
		break;
		case 7:
			#ifdef USE_CAPTURE_EXIT7
				pICPExt7 = this;
			#endif
			EXTIx_IRQn = EXTI9_5_IRQn; 
		break;
		case 8:
			#ifdef USE_CAPTURE_EXIT8
				pICPExt8 = this;
			#endif
			EXTIx_IRQn = EXTI9_5_IRQn; 
		case 9:
			EXTIx_IRQn = EXTI9_5_IRQn; 
			#ifdef USE_CAPTURE_EXIT9
				pICPExt9 = this;
			#endif
		break;
		case 10:
			#ifdef USE_CAPTURE_EXIT10
				pICPExt10 = this;
			#endif
			EXTIx_IRQn = EXTI15_10_IRQn; 
		break;		
		case 11:
			#ifdef USE_CAPTURE_EXIT11
				pICPExt11 = this;
			#endif
			EXTIx_IRQn = EXTI15_10_IRQn; 
		break;		
		case 12:
			#ifdef USE_CAPTURE_EXIT12
				pICPExt12 = this;
			#endif
			EXTIx_IRQn = EXTI15_10_IRQn; 
		break;		
		case 13:
			#ifdef USE_CAPTURE_EXIT13
				pICPExt13 = this;
			#endif
			EXTIx_IRQn = EXTI15_10_IRQn; 
		break;		
		case 14:
			#ifdef USE_CAPTURE_EXIT14
				pICPExt14 = this;
			#endif	
			EXTIx_IRQn = EXTI15_10_IRQn; 
		break;
		case 15:
			EXTIx_IRQn = EXTI15_10_IRQn; 
			#ifdef USE_CAPTURE_EXIT15
				pICPExt15 = this;
			#endif
		break;
	}	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(priorityGroup[itGroup]);		               //one project should has only one priority group
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn;	                 //IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prePriority;//preemption priority 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;       //sub priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	//Stpe 3: EXTI Initialize
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_ClearITPendingBit(u32(mExitPin));							        //clear exti interrupt flag bit
	EXTI_InitStructure.EXTI_Line = u32(mExitPin);					 	      //select exti line
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //interrupt
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//trigger: both rising and falling edge
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;	
	EXTI_Init(&EXTI_InitStructure);
		
	//?????
	mInterval = 0;
}

void InputCapture_EXIT::IRQ()
{
	static float riseTime = 0;
	if(GPIO_ReadInputDataBit(mExitPort,mExitPin))	//rising edge interrupt
		riseTime = tskmgr.Time();                   //rising edge time
	else											                    //falling edge interrupt
		mInterval = (tskmgr.Time() - riseTime)*1000;//falling edge time
}

float InputCapture_EXIT::Value()
{
	return mInterval;
}


