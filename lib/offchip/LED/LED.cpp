#include "LED.h"


///构造函数
///@param gpio GPIO类对象，分配片上资源
///@param highLevelOn  是否输出高电平时为LED打开
/////////////////////////
LED::LED(GPIO &gpio,bool highLevelOn)
	:mGPIO(gpio),times(0),time_old(0)
{
	if(highLevelOn)
		mLevelOn=1;
	else
		mLevelOn=0;
	Off();
}


////////////////////////
///翻转（开-->关 ， 关-->开）
///////////////////////
void LED::Toggle()
{
	if(mGPIO.GetLevel())
		mGPIO.SetLevel(0);
	else
		mGPIO.SetLevel(1);
}

//////////////////////
///打开LED
/////////////////////
void LED::On()
{
	mGPIO.SetLevel(mLevelOn);
}

/////////////////////
///关闭LED
////////////////////
void LED::Off()
{
	mGPIO.SetLevel(mLevelOn^1);
}

////////////////////////
///闪烁n次
///@param time 闪烁次数
///@param Interval 闪烁间隔 (ms)
///@param wait 是否等待闪烁完毕后再结束函数
///////////////////////
void LED::Blink(uint16_t time,float interval,bool wait)
{
	if(wait){
		for(uint8_t i=0;i<time;++i)
		{
			Toggle();
			TaskManager::DelayMs(interval);
		}
		return;
	}
	if(time!=0 && times>=time)
	{
		times = 0;
		return;
	}
	if(TaskManager::Time() - time_old > interval/1000.0)
	{
		time_old = TaskManager::Time();
		++times;
		Toggle();
	}	
}
////////////////////////
///两个灯一起闪烁n次
///@param 另一个灯的引用
///@param time 闪烁次数
///@param Interval 闪烁间隔(ms)
///////////////////////
void LED::Blink2(LED &led,uint8_t time,uint16_t interval)
{
	for(uint8_t i=0;i<time;++i)
	{
		On();
		led.On();
		TaskManager::DelayMs(interval);
		Off();
		led.Off();
		TaskManager::DelayMs(interval);
	}
}

////////////////////////
///两个灯交替闪烁n次
///@param 另一个灯的引用
///@param time 闪烁次数
///@param Interval 闪烁间隔(ms)
///////////////////////
void LED::Blink3(LED &led,uint8_t time,uint16_t interval)
{
	for(uint8_t i=0;i<time;++i)
	{
		On();
		led.Off();
		TaskManager::DelayMs(interval);
		Off();
		led.On();
		TaskManager::DelayMs(interval);
	}
	led.Off();
}

