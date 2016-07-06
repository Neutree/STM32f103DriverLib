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
#include "BLDCMotor.h"


#include "Gimbal.h"
/************************************硬件定义*************************************/
//Timer T1(TIM1,1,2,3); //使用定时器计，溢出时间:1S+2毫秒+3微秒
USART com(1,115200);
I2C i2c2(2); 
mpu6050 mpu6050(i2c2,500);
//HMC5883L mag(i2c2);
PWM pwm2(TIM2,1,1,1,1,20000);  //开启时钟2的4个通道，频率2Whz
PWM pwm3(TIM3,1,1,0,0,20000);  //开启时钟3的2个通道，频率2Whz
PWM pwm4(TIM4,1,1,1,0,20000);  //开启时钟4的3个通道，频率2Whz
//InputCapture_TIM t4(TIM4, 400, true, true, true, true);
//InputCapture_EXIT ch1(GPIOB,6);
ADC voltage(4); //读取电压值
//flash InfoStore(0x08000000+100*MEMORY_PAGE_SIZE,true);     //flash

//LED
GPIO ledRedGPIO(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
GPIO ledBlueGPIO(GPIOB,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red
LED ledBlue(ledBlueGPIO);//LED blue

//BLDC Motor
BLDCMotor motorRoll(&pwm2,1,&pwm2,2,&pwm2,3,0.6);  //roll motor
BLDCMotor motorPitch(&pwm2,4,&pwm3,1,&pwm3,2,0.45); //pitch motor
BLDCMotor motorYaw(&pwm4,1,&pwm4,2,&pwm4,3,0.55);   //yaw motor


/**************************************************************************/


/*************************全局变量*****************************************/

Gimbal gimbal(mpu6050,motorRoll,motorPitch,motorYaw,voltage);

/**************************************************************************/


/**
  *系统初始化
  *
  */
void init()
{
	ledBlue.On();
	ledRed.Off();
	
	gimbal.Init();
	
//	//测试磁力计是否存在
//	if(!mag.TestConnection(false))
//		com<<"mag connection error\n";
//	//初始化磁力计
//	mag.Init();
	
}


/**
  *循环体
  *
  */
void loop()
{
	static double record_tmgTest=0,record_tmgTest2 = 0; //taskmanager时间 测试
	
	
	ledBlue.Blink(0,0.5,false);
	
	if(tskmgr.TimeSlice(record_tmgTest,0.002)) //每0.002秒执行一次
	{
		gimbal.UpdateIMU();
		gimbal.UpdateMotor();
	}
	if(tskmgr.TimeSlice(record_tmgTest2,0.5)) //每1秒执行一次，输出电源值
	{
		if(gimbal.IsCalibrated())
		{
			ledRed.Toggle();
		//	LOG("voltage:");LOG(gimbal.UpdateVoltage(4,5.1,1,12));LOG("\n");
		}
		else if(gimbal.IsCalibrating())
			LOG(".");
		//com<<"kp:"<<gimbal.mPIDRoll.GetKp()<<"\t"<<gimbal.mPIDRoll.GetKi()<<"\t"<<gimbal.mPIDRoll.GetKd()<<"\n";
	}
	
	if(com.ReceiveBufferSize()>0)
	{
		u8 temp;
		com.GetReceivedData(&temp,1);
		if(temp == '0')
			gimbal.mPIDRoll.AddKp(5);
		else if(temp == '.')
			gimbal.mPIDRoll.AddKp(-5);
		else if(temp == '1')
			gimbal.mPIDRoll.AddKi(1);
		else if(temp=='2')
			gimbal.mPIDRoll.AddKi(-1);
		else if(temp=='4')
			gimbal.mPIDRoll.AddKd(0.5);
		else if(temp=='5')
			gimbal.mPIDRoll.AddKd(-0.5);
		
	}
}




int main()
{
	TaskManager::DelayS(2);
	init();
	while(1)
	{
		loop();
	}
}
