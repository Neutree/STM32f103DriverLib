/**
  ******************************************************************************
  * @file    Timer.h 
  * @author  lissettecarlr
  * @version V1.0
             V1.1 增加开关中断功能
			 V1.2 增加清除CNT计数器的值的功能
			 
  * @date    10/25/2015(create)  
			 10/28/2015(Debug)
			 11/02/2015(add)
  * @brief   必须配套Interrupt文件使用，用户如需需要编写中断函数，直接编写
TIMX	CH1		CH2		CH3		CH4
1		PA8		PA9		PA10 	PA11
2		PA0		PA1		PA2		PA3
3		PA6		PA7		PB0		PB1
4		PB6		PB7		PB8		PB9
  ******************************************************************************
*/

#ifndef __TIMER_H
#define __TIMER_H

/* Includes ------------------------------------------------------------------*/
extern "C"{
#include "stm32f10x.h"
}

/* define -------------------------------------------------------------------*/
//#define COEFFICIENT  66446
#define COEFFICIENT  911          
//  65535/72
/* class----------------------------------------------------------------------*/

class Timer{
	private:
		u16 mArr;//计数器初值
		u16 mPsc;//计数器预分频
		TIM_TypeDef *mTempTimer;//时钟选择
		void Conversion(u16 s,u16 ms,u16 us);//将时分秒转换为初值和预分频
		
	public:
	////////////////////////////////////////
	///定时器初始化，默认定时器1，定时1ms
	///@param timer 选择使用的定时器
	///@param Prioritygroup 中断分组
	///@param preemprionPriority 抢占优先级
	///@param subPriority 响应优先级
	///@param second 秒
	///@param millisecond 毫秒
	///@param microsecond 微秒
	////////////////////////////////////////
		Timer(TIM_TypeDef *timer=TIM1,u16 second=0,u16 millisecond=1,u16 microsecond=0,u8 Prioritygroup=2,u8 preemprionPriority=2,u8 subPriority=2);
	
	////////////////////
	///开启定时器
	///////////////////
		void Start();
	
	///////////////////
	///关闭定时器
	//////////////////
		void Stop();
	/////////////////////////////////////////
	///关闭定时器
	///@param bool true 开启中断  false 关闭中断 
	//////////////////////////////////////////
		void OnOrOffIrq(bool Switch);

	///////////////////
	///清空计数器的值
	//////////////////
	void ClearCNT(void);
	
		 		
};

#endif
