#ifndef _LED_H
#define _LED_H

#include "GPIO.h"
#include "TaskManager.h"
class LED
{
	public:
		////////////////////////
		///构造函数
		///@param gpio GPIO类对象，分配片上资源
		///@param highLevelOn  是否输出高电平时为LED打开
		/////////////////////////
		LED(GPIO &gpio,bool highLevelOn=false);
	
	
		////////////////////////
		///翻转（开-->关 ， 关-->开）
		///////////////////////
		void Toggle();
	
		//////////////////////
		///打开LED
		/////////////////////
		void On();
	
		/////////////////////
		///关闭LED
		////////////////////
		void Off();
		
		////////////////////////
		///闪烁n次
		///@param time 闪烁次数,若为0，则为无线闪烁（只适用于非等待方式）
		///@param Interval 闪烁间隔(ms)
		///@param wait 是否等待闪烁完毕后再结束函数
		///////////////////////
		void Blink(uint16_t time,float interval,bool wait);
		
		
		////////////////////////
		///两个灯一起闪烁n次,闪烁完后结束
		///@param 另一个灯的引用
		///@param time 闪烁次数
		///@param Interval 闪烁间隔(ms)
		///////////////////////
		void Blink2(LED &led,uint8_t time,uint16_t interval);
		
		
		////////////////////////
		///两个灯交替闪烁n次,闪烁完后结束
		///@param 另一个灯的引用
		///@param time 闪烁次数
		///@param Interval 闪烁间隔(ms)
		///////////////////////
		void Blink3(LED &led,uint8_t time,uint16_t interval);
		
		
	private:
		//////////////////////////
		///GPIO的类的实例对象引用
		//////////////////////////
		GPIO & mGPIO;
		////////////////////////
		///标志开启LED时取的电平高低
		////////////////////////
		unsigned char mLevelOn;
	
		uint16_t times;
		double time_old;
	
	
	
};


#endif


