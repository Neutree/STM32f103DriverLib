#ifndef _F103_PWM_H_
#define _F103_PWM_H_
 
#include "stm32f10x.h"

class PWM
{
private:
	u32 _timx_rcc;				//timer rcc
	u32 _gpio_rcc;        //gpio rcc
	TIM_TypeDef *_timx;   //timer 
	GPIO_TypeDef *_port;  //gpio
	u16 _pin1;            //ch1 gpio pin
	u16 _pin2;            //ch2 gpio pin
	u16 _pin3;            //ch3 gpio pin
	u16 _pin4;            //ch4 gpio pin
	bool _enCh1;          //ch1 enable flag
	bool _enCh2;          //ch2 enable flag
	bool _enCh3;          //ch3 enable falg
	bool _enCh4;          //ch4 enable falg
	u16 _frqence;         //freqence 
	u32 _period;          //period
	u16 _prescaler;       //prescaler
public:
	PWM(TIM_TypeDef *timx,bool enCh1,bool enCh2,bool enCh3, bool enCh4, u16 frq);
	void Initialize(TIM_TypeDef *timx,bool enCh1,bool enCh2,bool enCh3, bool enCh4, u16 frq);
	void SetDuty(float ch1,float ch2, float ch3, float ch4=0);
	void SetDuty(u8 chNum,float ch2);
};
 

 
#endif
