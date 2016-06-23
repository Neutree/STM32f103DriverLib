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

Timer T1(TIM1,1,2,3); //使用定时器计，溢出时间:1S+2毫秒+3微秒
USART com(1,115200,false);
I2C abc(2); 
PWM t2(TIM2,1,1,1,1,20000);  //开启时钟2的4个通道，频率2Whz
InputCapture_TIM t4(TIM4, 400, true, true, true, true);
InputCapture_EXIT ch1(GPIOB,6);
ADC pressure(1); //PA1读取AD值
flash InfoStore(0x08000000+100*MEMORY_PAGE_SIZE,true);
GPIO Led_red(GPIOB,8,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

int main()
{
	while(1)
	{
		double record_tmgTest=0; //taskmanager时间 测试
		
		if(tskmgr.TimeSlice(record_tmgTest,2)) //每两秒执行一次
		{
				//lalalala
		}
		
	}
	
}
