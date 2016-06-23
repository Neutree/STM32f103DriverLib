#include "PWM.h"

PWM::PWM(TIM_TypeDef *timx,bool enCh1,bool enCh2,bool enCh3, bool enCh4, u16 frq)
{
	Initialize(timx,enCh1,enCh2,enCh3, enCh4, frq);
}

void PWM::Initialize(TIM_TypeDef *timx,bool enCh1,bool enCh2,bool enCh3, bool enCh4, u16 frq)
{
	_timx = timx;
	_enCh1 = enCh1;
	_enCh2 = enCh2;
	_enCh3 = enCh3;
	_enCh4 = enCh4;
	_frqence = frq;
	
	
	if(_timx==TIM1)
	{
		_timx_rcc = RCC_APB2Periph_TIM1;
	}
	else if(_timx==TIM2)
	{
		_timx_rcc = RCC_APB1Periph_TIM2;
		_gpio_rcc = RCC_APB2Periph_GPIOA;
		_port = GPIOA;
		_pin1 = GPIO_Pin_0;
		_pin2 = GPIO_Pin_1;
		_pin3 = GPIO_Pin_2;
		_pin4 = GPIO_Pin_3;
	}
	else if(_timx==TIM3)
	{
		_timx_rcc = RCC_APB1Periph_TIM3;
		_gpio_rcc = RCC_APB2Periph_GPIOA;
		_port = GPIOA;
		_pin1 = GPIO_Pin_6;
		_pin2 = GPIO_Pin_7;
		_pin3 = GPIO_Pin_0;
		_pin4 = GPIO_Pin_1;
	}
	else if(_timx==TIM4)
	{
		_timx_rcc = RCC_APB1Periph_TIM4;
		_gpio_rcc = RCC_APB2Periph_GPIOB;
		_port = GPIOB;
		_pin1 = GPIO_Pin_6;
		_pin2 = GPIO_Pin_7;
		_pin3 = GPIO_Pin_8;
		_pin4 = GPIO_Pin_9;
	}
	
	//RCC
	if(_timx==TIM1) RCC_APB2PeriphClockCmd(_timx_rcc,ENABLE);
	else            RCC_APB1PeriphClockCmd(_timx_rcc,ENABLE);
	RCC_APB2PeriphClockCmd(_gpio_rcc,ENABLE);
	
	//GPIO
	if(!_enCh1) _pin1 = 0;
	if(!_enCh2) _pin2 = 0;
	if(!_enCh3) _pin3 = 0;
	if(!_enCh4) _pin4 = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	if(_timx==TIM3)
	{
			GPIO_InitStructure.GPIO_Pin = _pin3 | _pin4;
			GPIO_Init(GPIOB,&GPIO_InitStructure);
			_pin3 = 0;
			_pin4 = 0;
	}
	GPIO_InitStructure.GPIO_Pin = _pin1 | _pin2 | _pin3 | _pin4;
	GPIO_Init(_port,&GPIO_InitStructure);
	
	
	//TIMx
	
	u32 res = 72000000%_frqence;	
	u32 multi = 72000000/_frqence;
	if(res>_frqence/2) multi++;
	_prescaler = 1;
	_period = multi;
	while(_period>=65535)
	{
		_period = multi/(++_prescaler);
	}
	//TIM_InternalClockConfig(_timx);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;//定义定时器的结构体变量
	TIM_TimeBaseStruct.TIM_Period = _period-1;//初值
	TIM_TimeBaseStruct.TIM_Prescaler = _prescaler-1;//预分频
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;//0;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上
	TIM_TimeBaseInit(_timx,&TIM_TimeBaseStruct);//初始化TIM4
	
	//PWM
	TIM_OCInitTypeDef TIM_OCInitStructure;	//？定义pwm的结构体变量	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							//定时器模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;			//输出极性:占空比是高电平OR低电平
	
	TIM_OCInitStructure.TIM_Pulse = 0;													//占空比
	if(_enCh1) TIM_OC1Init(_timx,&TIM_OCInitStructure);//初始化TIM4-通道1~4
	if(_enCh2) TIM_OC2Init(_timx,&TIM_OCInitStructure);
	if(_enCh3) TIM_OC3Init(_timx,&TIM_OCInitStructure);
	if(_enCh4) TIM_OC4Init(_timx,&TIM_OCInitStructure);	
	if(_enCh1) TIM_OC1PreloadConfig(_timx,TIM_OCPreload_Enable);//使能预装载器 通道1~4
	if(_enCh2) TIM_OC2PreloadConfig(_timx,TIM_OCPreload_Enable);
	if(_enCh3) TIM_OC3PreloadConfig(_timx,TIM_OCPreload_Enable);	
	if(_enCh4) TIM_OC4PreloadConfig(_timx,TIM_OCPreload_Enable);
	TIM_Cmd(_timx,ENABLE);	
}
void PWM::SetDuty(u8 chNum,float duty)
{
	switch(chNum)
	{
		case 1:
			if(_enCh1) _timx->CCR1 = _period*duty/100.0f;
			break;
		case 2:
			if(_enCh2) _timx->CCR2 = _period*duty/100.0f;
			break;
		case 3:
			if(_enCh3) _timx->CCR3 = _period*duty/100.0f;
			break;
		case 4:
			if(_enCh4) _timx->CCR4 = _period*duty/100.0f;
			break;
	}
}
void PWM::SetDuty(float ch1,float ch2, float ch3, float ch4)
{
	if(_enCh1) _timx->CCR1 = _period*ch1/100.0f;
	if(_enCh2) _timx->CCR2 = _period*ch2/100.0f;
	if(_enCh3) _timx->CCR3 = _period*ch3/100.0f;
	if(_enCh4) _timx->CCR4 = _period*ch4/100.0f;
}
