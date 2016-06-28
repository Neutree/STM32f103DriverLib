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
#include "mpu6050.h"
#include "HMC5883L.h"

//Timer T1(TIM1,1,2,3); //使用定时器计，溢出时间:1S+2毫秒+3微秒
USART com(1,115200,true);
I2C i2c2(2); 
mpu6050 mpu6050(i2c2);
HMC5883L mag(i2c2);
//PWM pwm2(TIM2,1,1,1,1,20000);  //开启时钟2的4个通道，频率2Whz
//PWM pwm3(TIM3,1,1,0,0,20000);  //开启时钟3的2个通道，频率2Whz
//PWM pwm4(TIM4,1,1,1,0,20000);  //开启时钟4的3个通道，频率2Whz
//InputCapture_TIM t4(TIM4, 400, true, true, true, true);
//InputCapture_EXIT ch1(GPIOB,6);
ADC voltage(4); //读取电压值
//flash InfoStore(0x08000000+100*MEMORY_PAGE_SIZE,true);     //flash

//LED
GPIO ledRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
GPIO ledBlueGPIO(GPIOB,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red
LED ledBlue(ledBlueGPIO);//LED blue

/**
  *系统初始化
  *
  */
void init()
{
	ledBlue.On();
	ledRed.Off();
	//初始化MPU6050（三轴加速度计、三轴角速度计）
	mpu6050.Init();
	//测试磁力计是否存在
	if(!mag.TestConnection(false))
		com<<"mag connection error\n";
	//初始化磁力计
	mag.Init();
}

/**
  *循环体
  *
  */
void loop()
{
	static double record_tmgTest=0; //taskmanager时间 测试
	if(tskmgr.TimeSlice(record_tmgTest,0.2)) //每0.2秒执行一次
	{
		ledRed.Toggle();
		//com<<voltage.Voltage_I(4,5.1,1,12)<<"\n";
		if(MOD_ERROR== mpu6050.Update())
		{
			com<<"mpu6050 error\n\n\n";
		}
		if(MOD_ERROR== mag.Update())
		{
			com<<"mag error\n\n\n";
		}
		com<<mpu6050.GetTempRaw()<<"  "<<mpu6050.GetAccRaw().z<<"\t"<<mag.Heading()<<"\t"<<"\tinterval:"<<mpu6050.GetUpdateInterval()<<"\t"<<mag.GetUpdateInterval()<<"\n";
	}
	
	ledBlue.Blink(0,0.5,false);
}




int main()
{
	init();
	while(1)
	{
		loop();
	}
}
