#include "GPIO.h"
#include "Delay.h"

GPIO RedLed(GPIOB,3,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO led(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO led2(GPIOB,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

int main()
{
		bool Level = false;
		RedLed.DisableSWJTAG(false,true);//PB3要使用通用功能，需要先关闭JTAG功能
		RedLed.SetLevel(1);//设置管脚电平为高
		while(1)
		{
			RedLed.Toggle(); //翻转电平
			led.Toggle();
			led2.Toggle();
			Level=RedLed.GetLevel();//获取当前管脚电平
			Delay::Ms(100);
			
		}
}

