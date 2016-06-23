/********************************************************************************
  * @file    InputCapture.h 
  * @author  lissettecarlr(creat)
  * @version V1.0
  * @date    10/26/2015          
  * @brief   TIMx	CH1		CH2		CH3	 	CH4
	*		        1		PA8		PA9		PA10 	PA11
	*	          	2		PA0		PA1		PA2		PA3
	*		        3		PA6		PA7		PB0		PB1
	*		        4		PB6		PB7		PB8		PB9
	* @attention 	USART1.Rx = TIM1.CH2, USART1.Tx = TIM1.CH3
  ******************************************************************************
*/

#ifndef __INPUT_CAPTURE_H_
#define __INPUT_CAPTURE_H_


#include "stm32f10x.h"

class InputCapture_TIM
{
private:
	TIM_TypeDef *_TIMx;  //timer for capture function
	double _resolution;  //the time for one tick of counter (ms/tick)
	u16 _ch1Cnt;  //count of ticks for channel1
	u16 _ch2Cnt;  //count of ticks for channel2
	u16 _ch3Cnt;  //count of ticks for channel3
	u16 _ch4Cnt;  //count of ticks for channel4
	
	u16 _ch1Pin;  //gpio pin for channel1
	u16 _ch2Pin;  //gpio pin for channel2
	u16 _ch3Pin;  //gpio pin for channel3
	u16 _ch4Pin;  //gpio pin for channel4
	bool _enCh1;  //channel 1 enable flag
	bool _enCh2;  //channel 2 enable flag
	bool _enCh3;  //channel 3 enable flag
	bool _enCh4;  //channel 4 enable flag

	GPIO_TypeDef *_port, *_port2; //gpio port
	u8 _IRQn;       //IRQ channel
	u32 _timer_rcc; //clock of timer
	u32 _gpio_rcc;  //clock of gpio
	u32 _prescaler; //prescaler of timer
	u32 _period;    //period of timer
public:				 
	InputCapture_TIM(TIM_TypeDef *TIMx, u16 minHz, bool enCh1=false, bool enCh2=false, bool enCh3=false, bool enCh4=false, u8 intGroup=2,u8 prePriority=2,u8 subPriority=2);	
	void Start();	    //start timer
	void Stop();      //stop timer
	void IRQ();	      //interrupt response function
	double operator[](u8 chNum); //return capture channel value, ms
};


#endif
