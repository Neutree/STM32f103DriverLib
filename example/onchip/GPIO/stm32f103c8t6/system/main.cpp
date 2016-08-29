#include "GPIO.h"
#include "Delay.h"

GPIO RedLed(GPIOA,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

int main()
{
		bool Level = false;
		RedLed.SetLevel(1);//设置管脚电平为高
		while(1)
		{
			RedLed.Toggle(); //翻转电平
			Level=RedLed.GetLevel();//获取当前管脚电平
			Delay::Ms(100);
			
		}
}

