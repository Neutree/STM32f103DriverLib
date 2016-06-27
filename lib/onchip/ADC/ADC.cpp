#include "ADC.h"

//  ADC Table Initialize:           CH0           CH1        CH2         CH3         CH4         CH5         CH6         CH7         CH8         CH9
u16  ADC::_pin[MAX_ADC_CHANNEL]  = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7, GPIO_Pin_0, GPIO_Pin_1};
bool ADC::_enCh[MAX_ADC_CHANNEL] = {false,      false,      false,      false,      false,      false,      false,      false,      false,      false};
u8   ADC::_chMap[MAX_ADC_CHANNEL]= {0};     //map channel number to the index of _adcValue
u16  ADC::_adcValue[MAX_ADC_CHANNEL] = {0}; //adc value for each channel

//constructor: ADC and DMA initialize
ADC::ADC(u8 ch0Num, u8 ch1Num, u8 ch2Num, u8 ch3Num, u8 ch4Num, u8 ch5Num, u8 ch6Num, u8 ch7Num, u8 ch8Num, u8 ch9Num)
{
	//Step 1: set adc channel enable flag
	if(ch0Num<MAX_ADC_CHANNEL) _enCh[ch0Num] = true;
	if(ch1Num<MAX_ADC_CHANNEL) _enCh[ch1Num] = true;
	if(ch2Num<MAX_ADC_CHANNEL) _enCh[ch2Num] = true;
	if(ch3Num<MAX_ADC_CHANNEL) _enCh[ch3Num] = true;
	if(ch4Num<MAX_ADC_CHANNEL) _enCh[ch4Num] = true;
	if(ch5Num<MAX_ADC_CHANNEL) _enCh[ch5Num] = true;
	if(ch6Num<MAX_ADC_CHANNEL) _enCh[ch6Num] = true;
	if(ch7Num<MAX_ADC_CHANNEL) _enCh[ch7Num] = true;
	if(ch8Num<MAX_ADC_CHANNEL) _enCh[ch8Num] = true;
	if(ch9Num<MAX_ADC_CHANNEL) _enCh[ch9Num] = true;
	
	//Step 2: initialize rcc and gpio
	u16 pinA = 0, pinB = 0, chCnt = 0;
	
	for(int i=0; i<MAX_ADC_CHANNEL; i++)
	{
		if(_enCh[i])//enabled adc channel
		{
			chCnt++;                 //count the total number of enabled adc channel
			if(i<8)	pinA |= _pin[i]; //all enabled adc pin in GPIOA
			else    pinB |= _pin[i]; //all enabled adc pin in GPIOB
		}
	}	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //gpio pin mode: anology input
	if(pinA) //initialize rcc and gpio in GPIOA
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//open the PinA clock
		GPIO_InitStructure.GPIO_Pin = pinA;									//Pin
		GPIO_Init(GPIOA,&GPIO_InitStructure);			
	}
	if(pinB) //initialize rcc and gpio in GPIOB
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
		GPIO_InitStructure.GPIO_Pin = pinB;									
		GPIO_Init(GPIOB,&GPIO_InitStructure);					
	}
	
	//Step 3: Initialize DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                         //DMA1 clock
	DMA_InitTypeDef  DMA_InitStructure;
	DMA_DeInit(DMA1_Channel1);                                                 //default set DMA1
	DMA_InitStructure.DMA_PeripheralBaseAddr = u32(&(ADC1->DR));							 //ADC1->DR address
	DMA_InitStructure.DMA_MemoryBaseAddr = u32(_adcValue);	                   //first address for save adc value
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;												 //ÒÔÍâÉèÎªDMAÔ´
	DMA_InitStructure.DMA_BufferSize = chCnt;                                  //Êý¾Ý×ªÒÆÁ¿ÎªmAdcTotal£¨1Â·AD£©
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	         //ÍâÉèµØÖ·²»µÝÔö
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			    				 //ÄÚ´æµØÖ·²»µÝÔö
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//½ÓÏÂÀ´µÄ¸ú²É¼¯Ò»Â·ADÒ»Ñù
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);            //Initialize DMA channel
	DMA_Cmd(DMA1_Channel1, ENABLE);                         //enable DMA channel
	
	//Step 4: Initialize ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);		//ADC1 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //alternativ function clock
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	                    //adc clock div, adc clock should less than 14M 
	
	ADC_InitTypeDef  ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;												//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 									//Continuous Convertion
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//
	ADC_InitStructure.ADC_NbrOfChannel = chCnt;											    //total adc channel	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	for(int i=MAX_ADC_CHANNEL-1; i>=0; i--)
	{
		if(_enCh[i])
		{
			ADC_RegularChannelConfig(ADC1, i, chCnt--, ADC_SampleTime_239Cycles5);
			_chMap[i] = chCnt;  //map adc channel number to adc value array index _adcValue[_chMap[ADC Channel Number]]
		}
	}
	ADC_DMACmd(ADC1,ENABLE);                   //enable DMA for ADC1
	ADC_Cmd(ADC1,ENABLE);                      //enable ADC1
	ADC_ResetCalibration(ADC1);								 //reset ADC1
	while(ADC_GetResetCalibrationStatus(ADC1));//waiting for adc1 reset complete
	ADC_StartCalibration(ADC1);								 //calibrate adc1 
	while(ADC_GetCalibrationStatus(ADC1));		 //waiting for adc1 calibrate complete
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);     //start adc1	
}
//get adc channel votage value
double ADC::operator[](u8 chNum)
{
	if(chNum >= MAX_ADC_CHANNEL)//adc channel number check
		return 0;      
	if(_enCh[chNum]) //if adc channel enabled
		return _adcValue[_chMap[chNum]]*0.0008056640625;// 0.0008056640625 = 3.3v / 4096
	else             //adc channel not enabled
		return 0;
}

/**
  *获取电压值
  *@param channelNumber  ADC通道号
  *@param resister_a   电阻a的值，接近电源的电阻值
  *@param resister_b   电阻b的值，接近地的电阻值
  *@param fullRange   电压最大值
  *@retval 电压值
  */
float ADC::Voltage_I(uint8_t channelNumber,float resister_a,float resister_b,float fullRange)
{
	static float voltage = fullRange;
	voltage += 0.002*((*this)[channelNumber]*((resister_a+resister_b)*1.0/resister_b) - voltage);
	return voltage;
}


