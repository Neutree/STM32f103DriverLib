#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"


/**
 *
 *                        ADC端口 和 STM32f103c8t6板子的对应引脚
 *                      （stm32f10x系列板若有以下引脚也可兼容使用）
 *                          PA0---ADC12_IN0   PB0---ADC12_IN8
 *                          PA1---ADC12_IN1   PB1---ADC12_IN9
 *                          PA2---ADC12_IN2   PC0---ADC12_IN10
 *                          PA3---ADC12_IN3   PC1---ADC12_IN11
 *                          PA4---ADC12_IN4   PC2---ADC12_IN12
 *                          PA5---ADC12_IN5   PC3---ADC12_IN13
 *                          PA6---ADC12_IN6   PC4---ADC12_IN14
 *                          PA7---ADC12_IN7   PC5---ADC12_IN15
 *
 */


#define MAX_ADC_CHANNEL 10

class ADC
{
private:
	static u16  _pin[MAX_ADC_CHANNEL];		  //gpio pin for adc channel
	static bool _enCh[MAX_ADC_CHANNEL];     //enable flag for adc channel
	static u8   _chMap[MAX_ADC_CHANNEL];    //map channel number to the index of _adcValue
	static u16  _adcValue[MAX_ADC_CHANNEL]; //adc value for each channel
public:
	ADC(u8 ch0Num, u8 ch1Num=0xFF, u8 ch2Num=0xFF, u8 ch3Num=0xFF, u8 ch4Num=0xFF, u8 ch5Num=0xFF, u8 ch6Num=0xFF, u8 ch7Num=0xFF, u8 ch8Num=0xFF, u8 ch9Num=0xFF);
	double operator[](u8 chNum);
	float Voltage_I(uint8_t channelNumber, float resister_a,float resister_b,float fullRange);
};

#endif

