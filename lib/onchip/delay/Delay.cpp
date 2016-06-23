/**
  *@file Delay.cpp
  *@author Neutree
  *@breif Delay Class
  *@copyright CQUT IOT Lib all rights reserved
  */

#include "Delay.h"




void Delay::Ms(uint16_t nms)
{
	
#ifdef DELAY_USE_SYSTICK
	u32 temp;
	 if(nms==0)
		 return;
	 SysTick->LOAD = 9000*nms;
	 SysTick->VAL=0X00;//清空计数器
	 SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
	 do
	 {
	  temp=SysTick->CTRL;//读取当前倒计数值
	 }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
	 SysTick->CTRL=0x00; //关闭计数器
	 SysTick->VAL =0X00; //清空计数器
#else
	u16 i=0;  
	while(nms--)
	{
		i=8000;  //自己定义
		while(i--) ;    
	}
#endif
}


void Delay::Us(uint32_t usec)
{
	u32 temp;
#ifdef DELAY_USE_SYSTICK
	 if(nus==0)
		 return;
	 SysTick->LOAD = 9*usec;
	 SysTick->VAL=0X00;//清空计数器
	 SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
	 do
	 {
	  temp=SysTick->CTRL;//读取当前倒计数值
	 }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
	 
	 SysTick->CTRL=0x00; //关闭计数器
	 SysTick->VAL =0X00; //清空计数器
	 
#else
	u16 i=0;  
	while(temp--)
	{
		i=10;  //自己定义
		while(i--) ;    
	}
#endif
}



void Delay::S(uint32_t ns)
{
	u32 temp;
#ifdef DELAY_USE_SYSTICK
	 u32 i;
	 if(ns==0)
		 return;
	 SysTick->LOAD = 9000000;//1S
	 SysTick->VAL=0X00;//清空计数器
	 SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
	for(i=0;i<ns;++i)
	{
		 do
		 {
		  temp=SysTick->CTRL;//读取当前倒计数值
		 }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
		 SysTick->LOAD =  9000000;//1S
	}
	 SysTick->CTRL=0x00; //关闭计数器
	 SysTick->VAL =0X00; //清空计数器
#else
	while(temp--)
	{
		Ms(1000);
	}
#endif
}

