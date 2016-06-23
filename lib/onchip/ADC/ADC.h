#ifndef _ADC_H_
#define _ADC_H_

#include "stm32f10x.h"

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
};

#endif

