/*
	夏普GP2Y1014 6823 PM2.5粉尘传感器，驱动方式主要用一路PWM（周期10ms，启动时间0.32ms）来控制LED灯
	然后才能从AD角读出电压值。
	有传感器结构图可以看出控制LED的是一个PNP三极管，所以当电平为低的时候导通，开启。
	原定计划是用PWM，然后再中断时间中启动ADC读取，但先使用延时的方式测试传感器

*/



#ifndef _SHARP_1014_PM2_5_H
#define _SHARP_1014_PM2_5_H

#include "stm32f10x.h"
#include "GPIO.h"
#include "TaskManager.h"

//暂时先用宏定义
#define ADC_PIN   		GPIO_Pin_2
#define ADC_GPIO  		GPIOA
#define ADC_CHANNEL   ADC_Channel_2

class sharp_1014_pm2_5{
	private:
		
		uint16_t AdcOriginalVol;//保存ADC寄存器的原始值
		float OriginalVol;//保存计算出的电压值	
		GPIO &mLed;
		
		bool ADC_init();
		uint16_t Get_ADC();
		float InVolOutDensity(float Vol);
	
	public:
		sharp_1014_pm2_5(GPIO &LED);
		float testGetVol();
		bool updata();//数据更新，最快更新时间：10ms一次
		float getOriginalVol(float NearVccR,float OtherR);//读取原始电压值，该值根据分压电阻决定
		
};



#endif

