#include "SHARP_1014_PM2_5.h"

/*---------------------------TEST----------------------------------*/
sharp_1014_pm2_5::sharp_1014_pm2_5(GPIO &LED):mLed(LED)
{
	 OriginalVol=0;
	 AdcOriginalVol=0;
	 ADC_init();
	
}

float sharp_1014_pm2_5::testGetVol()
{
	//延时测试方案: 管脚拉低，延时280us，读取AD，延时40us，拉高管脚，再延时9680 us。
		mLed.SetLevel(0);
		tskmgr.DelayUs(280);
		tskmgr.DelayUs(40);
		mLed.SetLevel(1);
		tskmgr.DelayUs(9680);
	
	return OriginalVol; 
}



//sharp_1014_pm2_5::sharp_1014_pm2_5(PWM &Led,ADC &Adc,u8 AdcCh):mLED(Led),mAdc(Adc)
//{
//	OriginalVol=0;
//	AdcChannel = AdcCh;
//}


bool sharp_1014_pm2_5::updata()//数据更新，最快更新时间：10ms一次
{
	mLed.SetLevel(0);
	tskmgr.DelayUs(280);
	AdcOriginalVol = Get_ADC();
	tskmgr.DelayUs(40);
	mLed.SetLevel(1);
	tskmgr.DelayUs(9680);
	return true;
}

float sharp_1014_pm2_5::getOriginalVol(float NearVccR,float OtherR)//读取原始电压值，该值根据分压电阻决定
{
	
	 	OriginalVol= (float)AdcOriginalVol*0.0008056640625; //将原始值转化为电压
		OriginalVol=OriginalVol*(NearVccR/OtherR+1);//由于使用了8.61:14.84的电阻分压，所以要转化为原始电压
		return OriginalVol;
}


float sharp_1014_pm2_5::InVolOutDensity(float Vol)
{
	 float a = 0.17;
	 float b = -0.1;
	 //由图得出一个一元二次方程
   return (a*Vol+b);		
	
}

bool sharp_1014_pm2_5::ADC_init()
{
	//时钟初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	//管脚初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(ADC_GPIO,&GPIO_InitStructure);
	//ADC初始化
	ADC_DeInit(ADC1);
	ADC_InitTypeDef ADC_init;
	ADC_init.ADC_Mode = ADC_Mode_Independent; //独立工作模式
	ADC_init.ADC_ScanConvMode = DISABLE; //单通道模式
	ADC_init.ADC_ContinuousConvMode = DISABLE; //单次转换模式
	ADC_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ;//软件触发
	ADC_init.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
	ADC_init.ADC_NbrOfChannel = 1;//1个通道
	ADC_Init(ADC1,&ADC_init);
	//设置ADC的一些通道规则
	ADC_Cmd(ADC1,ENABLE);
	
	//
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	return true;
	
}

uint16_t sharp_1014_pm2_5::Get_ADC()
{
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,1,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//启动转换等待转换完成
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return (uint16_t)ADC_GetConversionValue(ADC1);
}

