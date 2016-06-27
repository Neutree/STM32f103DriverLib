#include "stm32f10x.h"
#include "Configuration.h"
#include "TaskManager.h"
#include "USART.h"
#include "I2C.h"
#include "Timer.h"
#include "ADC.h"
#include "PWM.h"
#include "flash.h"
#include "InputCapture_TIM.h"
#include "InputCapture_EXIT.h"
#include "LED.h"

//Timer T1(TIM1,1,2,3); //使用定时器计，溢出时间:1S+2毫秒+3微秒
USART com(1,115200,false);
I2C abc(2); 
PWM pwm2(TIM2,1,1,1,1,20000);  //开启时钟2的4个通道，频率2Whz
PWM pwm3(TIM3,1,1,0,0,20000);  //开启时钟3的2个通道，频率2Whz
PWM pwm4(TIM4,1,1,1,0,20000);  //开启时钟4的3个通道，频率2Whz
//InputCapture_TIM t4(TIM4, 400, true, true, true, true);
InputCapture_EXIT ch1(GPIOB,6);
ADC pressure(1); //PA1读取AD值
flash InfoStore(0x08000000+100*MEMORY_PAGE_SIZE,true);     //flash

GPIO ledRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
GPIO ledBlueGPIO(GPIOB,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red
LED ledBlue(ledBlueGPIO);//LED blue

int main()
{
	ledBlue.On();
	ledRed.Off();
	double record_tmgTest=0; //taskmanager时间 测试
	while(1)
	{
		if(tskmgr.TimeSlice(record_tmgTest,0.2)) //每0.2秒执行一次
		{
			ledRed.Toggle();
//			ledBlue.Toggle();
		}
		ledBlue.Blink(0,0.5,false);
	}
}
