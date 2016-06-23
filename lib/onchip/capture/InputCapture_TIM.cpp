#include "InputCapture_TIM.h"
#include "Interrupt.h"

//constructor, initialize: rcc, gpio, timer, NVIC
InputCapture_TIM::InputCapture_TIM(TIM_TypeDef *TIMx, u16 minHz, bool enCh1, bool enCh2, bool enCh3, bool enCh4, u8 intGroup,u8 prePriority,u8 subPriority)
{
	_TIMx = TIMx;   //timer selected for input capture
	_enCh1 = enCh1;
	_enCh2 = enCh2;
	_enCh3 = enCh3;
	_enCh4 = enCh4;
	
	_ch1Cnt = _ch2Cnt = _ch3Cnt = _ch4Cnt = 0;
	
	//get gpio, rcc, and irq
	if(TIMx==TIM1)      //Timer 1	
	{
		_port = _port2 = GPIOA;             //gpio port 
		_ch1Pin = GPIO_Pin_8;               //gpio pin for ch1
		_ch2Pin = GPIO_Pin_9;               //gpio pin for ch2
		_ch3Pin = GPIO_Pin_10;              //gpio pin for ch3
		_ch4Pin = GPIO_Pin_11;              //gpio pin for ch4
		_IRQn = TIM1_CC_IRQn;               //timer IRQ channel
		_timer_rcc = RCC_APB2Periph_TIM1;   //clock for timer
		_gpio_rcc = RCC_APB2Periph_GPIOA;   //clock for gpio
		
		#ifdef USE_CAPTURE_TIM1
		pICP1 = this;
		#endif
	}
	else if(_TIMx==TIM2)//Timer 2
	{
		_port = _port2 = GPIOA;            //gpio port 
		_ch1Pin = GPIO_Pin_0;              //gpio pin for ch1
		_ch2Pin = GPIO_Pin_1;              //gpio pin for ch2
		_ch3Pin = GPIO_Pin_1;              //gpio pin for ch3
		_ch4Pin = GPIO_Pin_3;              //gpio pin for ch4
		_IRQn = TIM2_IRQn;                 //timer IRQ channel
		_timer_rcc = RCC_APB1Periph_TIM2;  //clock for timer
		_gpio_rcc = RCC_APB2Periph_GPIOA;  //clock for gpio
		
		#ifdef USE_CAPTURE_TIM2
		pICP2 = this;
		#endif
	}
	else if(_TIMx==TIM3)//Timer 3
	{
		_port = GPIOA;                     //gpio port
		_port2 = GPIOB;                    //gpio port 
		_ch1Pin = GPIO_Pin_6;              //gpio pin for ch1
		_ch2Pin = GPIO_Pin_7;              //gpio pin for ch2
		_ch3Pin = GPIO_Pin_0;              //gpio pin for ch3
		_ch4Pin = GPIO_Pin_1;              //gpio pin for ch4
		_IRQn = TIM3_IRQn;                 //timer IRQ channel
		_timer_rcc = RCC_APB1Periph_TIM3;  //clock for timer
		_gpio_rcc = RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB;  //clock for gpio		
		
		#ifdef USE_CAPTURE_TIM3
		pICP3 = this;
		#endif
	}
	else if(_TIMx==TIM4)//Timer 4
	{
		_port = _port2 = GPIOB;            //gpio port 
		_ch1Pin = GPIO_Pin_6;              //gpio pin for ch1
		_ch2Pin = GPIO_Pin_7;              //gpio pin for ch2
		_ch3Pin = GPIO_Pin_8;              //gpio pin for ch3
		_ch4Pin = GPIO_Pin_9;              //gpio pin for ch4
		_IRQn = TIM4_IRQn;                 //timer IRQ channel
		_timer_rcc = RCC_APB1Periph_TIM4;  //clock for timer
		_gpio_rcc = RCC_APB2Periph_GPIOB;  //clock for gpio
		
		#ifdef USE_CAPTURE_TIM4
		pICP4 = this;
		#endif
	}
	//RCC Initialize
	if(_TIMx==TIM1) //advance timer
	{
		RCC_APB2PeriphClockCmd(_timer_rcc | _gpio_rcc, ENABLE);
	}
	else //normal timer
	{
		RCC_APB1PeriphClockCmd(_timer_rcc, ENABLE);
		RCC_APB2PeriphClockCmd(_gpio_rcc, ENABLE);
	}
	//GPIO Initialize
	if(!enCh1) _ch1Pin = 0;
	if(!enCh2) _ch2Pin = 0;
	if(!enCh3) _ch3Pin = 0;
	if(!enCh4) _ch4Pin = 0;
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 

	GPIO_InitStructure.GPIO_Pin = _ch1Pin | _ch2Pin; 
	GPIO_Init(_port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = _ch3Pin | _ch4Pin; 
	GPIO_Init(_port2, &GPIO_InitStructure);

	
	//Timer Initialize
	u32 res = 72000000 % minHz;	
	u32 multi = 72000000/minHz;
	if(res>minHz/2) multi++;
	_prescaler = 1;
	_period = multi;
	while(_period>=65535)
	{
		_period = multi/(++_prescaler);
	}
	_resolution = 1000.0 / (minHz*_period);
	TIM_DeInit(_TIMx);                                         //default setting
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	TIM_BaseInitStructure.TIM_Period = _period-1;               //period 
	TIM_BaseInitStructure.TIM_Prescaler = _prescaler-1;          //Prescaler
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //clock divsion
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//count direction
	TIM_TimeBaseInit(_TIMx,&TIM_BaseInitStructure);	
	TIM_ClearFlag(_TIMx, TIM_FLAG_Update);//clear 

	//Input Capture Initialize
	TIM_ICInitTypeDef  TIM_ICInitStructure;	
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;   //both rising and falling edge
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     
	TIM_ICInitStructure.TIM_ICFilter = 0x0; 
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; //channel 1    
	TIM_ICInit(_TIMx, &TIM_ICInitStructure);
	TIM_ITConfig(_TIMx, TIM_IT_CC1, _enCh1?ENABLE:DISABLE);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; //channel 2   
	TIM_ICInit(_TIMx, &TIM_ICInitStructure);
	TIM_ITConfig(_TIMx, TIM_IT_CC2, _enCh2?ENABLE:DISABLE);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; //channel 3   
	TIM_ICInit(_TIMx, &TIM_ICInitStructure);
	TIM_ITConfig(_TIMx, TIM_IT_CC3, _enCh3?ENABLE:DISABLE);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; //channel 4   
	TIM_ICInit(_TIMx, &TIM_ICInitStructure);
	TIM_ITConfig(_TIMx, TIM_IT_CC4, _enCh4?ENABLE:DISABLE);	
  //NVIC Initialize	
	switch(intGroup)// interrupt group setting
	{
		case 0:		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		break;
		case 1:		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		break;
		case 2:		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		break;
		case 3:		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);		break;
		case 4:		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);		break;
	}
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = _IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prePriority; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_Cmd(_TIMx, ENABLE);
}

//start work
void InputCapture_TIM::Start()
{
	TIM_Cmd(_TIMx, ENABLE);//start timer
}
//stop work
void InputCapture_TIM::Stop()
{
	TIM_Cmd(_TIMx,DISABLE);//stop timer
}
//input capture interrupt response function
void InputCapture_TIM::IRQ()
{
  static u16 cnt1, cnt2, cnt3, cnt4, ccr;
	//channel 1
	if(_TIMx->SR & TIM_IT_CC1)//
	{
		ccr = _TIMx->CCR1;         //read capture counter register, clear intterupt flag
		_TIMx->SR &= ~TIM_FLAG_CC1OF;  //clear capture flag
		if(_port->IDR & _ch1Pin) 		   //rising edge interrupt
		{
			cnt1 = ccr;            //start tick
			_TIMx->CCER |= (1<<1); //
		}		
		else                          //falling edge interrupt
		{
			_TIMx->CCER &= ~(1<<1);
			if(ccr >= cnt1)        //within one period
			{
				_ch1Cnt = ccr - cnt1;
			}
			else                   //cross two period
			{
				_ch1Cnt = _period - cnt1 + ccr;
			}
		}
	}
	//channel 2
	if(_TIMx->SR & TIM_IT_CC2)//
	{
		ccr = _TIMx->CCR2;         //read capture counter register, clear intterupt flag
		if(_port->IDR & _ch2Pin) 		   //rising edge interrupt
		{
			cnt2 = ccr;            //start tick
			_TIMx->CCER |= (1<<5); //
		}		
		else                          //falling edge interrupt
		{
			_TIMx->CCER &= ~(1<<5);
			if(ccr >= cnt2)        //within one period
			{
				_ch2Cnt = ccr - cnt2;
			}
			else                   //cross two period
			{
				_ch2Cnt = _period - cnt2 + ccr;
			}
		}
	}
	//channel 3
	if(_TIMx->SR & TIM_IT_CC3)//
	{
		ccr = _TIMx->CCR3;         //read capture counter register, clear intterupt flag
		if(_port2->IDR & _ch3Pin) 		   //rising edge interrupt
		{
			cnt3 = ccr;            //start tick
			_TIMx->CCER |= (1<<9); //
		}		
		else                          //falling edge interrupt
		{
			_TIMx->CCER &= ~(1<<9);
			if(ccr >= cnt3)        //within one period
			{
				_ch3Cnt = ccr - cnt3;
			}
			else                   //cross two period
			{
				_ch3Cnt = _period - cnt3 + ccr;
			}
		}
	}
	//channel 4
	if(_TIMx->SR & TIM_IT_CC4)//
	{
		ccr = _TIMx->CCR4;         //read capture counter register, clear intterupt flag
		if(_port2->IDR & _ch4Pin) 		   //rising edge interrupt
		{
			cnt4 = ccr;            //start tick
			_TIMx->CCER |= (1<<13); //
		}		
		else                          //falling edge interrupt
		{
			_TIMx->CCER &= ~(1<<13);
			if(ccr >= cnt4)        //within one period
			{
				_ch4Cnt = ccr - cnt4;//falling count - rising count
			}
			else                   //cross two period
			{
				_ch4Cnt = _period - cnt4 + ccr; //T - rising count + falling count
			}
		}
	}
}//return caputre channel value, unit: ms
double InputCapture_TIM::operator[](u8 chNum)
{
	if(chNum==1)
		return _ch1Cnt*_resolution;
	else if(chNum==2)
		return _ch2Cnt*_resolution;
  else if(chNum==3)
		return _ch3Cnt*_resolution;	
	else if(chNum==4)
		return _ch4Cnt*_resolution;
	else
		return 0;
}

