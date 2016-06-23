#include "Timer.h"

Timer::Timer(TIM_TypeDef *timer,u16 second,u16 millisecond,u16 microsecond,u8 Prioritygroup,u8 preemprionPriority,u8 subPriority)
{

	//通过计算的出了ARR PSC
	uint8_t timerIrqChannel;
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	mTempTimer=timer;
	NVIC_InitTypeDef NVIC_InitStructure;
    Conversion(second,millisecond,microsecond);
	
	if(timer==TIM1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		timerIrqChannel=TIM1_UP_IRQn;
		
	}
	else if(timer==TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		timerIrqChannel=TIM2_IRQn;
	}
	else if(timer==TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		timerIrqChannel=TIM3_IRQn;
	}
	else if(timer==TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		timerIrqChannel=TIM4_IRQn;
	}
	else
	{
	
	}
		//TIM_InternalClockConfig(timer);
	
//		TIM_DeInit(timer);//将寄存器重设为缺省值
		TIM_BaseInitStructure.TIM_Period = mArr-1; //设置初值
		TIM_BaseInitStructure.TIM_Prescaler =mPsc-1;//设置预分频
		TIM_BaseInitStructure.TIM_ClockDivision = 0;//设置时钟分割
	  TIM_BaseInitStructure.TIM_RepetitionCounter=0;
		TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//设置计数方式
		TIM_TimeBaseInit(timer,&TIM_BaseInitStructure);
		TIM_ClearFlag(timer, TIM_FLAG_Update);//清空中断标识   写了就进不了中断了
		TIM_ITConfig(timer, TIM_IT_Update, ENABLE); //使能中断
	
	switch(Prioritygroup)//中断分组
	{
		case 0:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
			break;
		case 1:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
			break;
		case 2:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			break;
		default:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
			break;
		case 4:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
			break;
	}
		NVIC_InitStructure.NVIC_IRQChannel =timerIrqChannel;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemprionPriority;  //先占优先
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriority;  //从优先
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM_Cmd(mTempTimer, ENABLE);//开启定时器
	/**/
}

void Timer::Start()
{
	TIM_Cmd(mTempTimer, ENABLE);//开启定时器
}

void Timer::Stop()
{
	TIM_Cmd(mTempTimer,DISABLE);//关闭定时器
}



/*
算法思路：
	COEFFICIENT表示1分屏时最大的时间
首先判断是否可以用一分频解决，如果不能，则计算出相近的预分频
然后判断是是否能用此预分频得到一个整数初值，如果不能则预分频加一在继续查找
如果始终未查找到能够整除的预分频和初值，则使用一开始的初始预分频，然后计算出
一个相近的初值来代替。
*/	
void Timer::Conversion(u16 s,u16 ms,u16 us) //将时分秒转化为预分频和初值
{
	u32 time;
	u16 tempPsc;//用于暂存计算值
	u32 tempArr=0xfffff;
	
	
	time=s*1000000+ms*1000+us; //计算总时间 单位us
	if(time<COEFFICIENT) //如果一分频可以满足
		mPsc=1;
	else
		mPsc=time/COEFFICIENT; //计算出最相近的预分频
	
	tempPsc=mPsc;	//保存初次计算结果
	
//	当计算出的ARR没有小数且ARR小于0xffff时退出循环，如果mps大于了65535 也退出
	while( ((time*72)% mPsc!=0  ||  tempArr>0xffff ) && mPsc<=65535)//如果计算的初值是个整数，或者没有找到可以计算出整数的分频数
	{
		mPsc++;	
		tempArr=(time*72)/mPsc; //计算出初值
	}
	
	if(mPsc>=65535) //如果找到能够整除的分频值，则选用精度最大的分频值
	{
		mPsc=tempPsc;
		tempArr=(time*72)/mPsc; //计算出初值
	}
	else
		mArr=tempArr;
	
	
	
}

void Timer::OnOrOffIrq(bool Switch)
{
   if (Switch==true)
	   mTempTimer->DIER|=1<<0; //使能更新中断
   else 
	   mTempTimer->DIER&=0<<0;  //关闭更新中断
}

void Timer::ClearCNT(void)
{
   mTempTimer->CNT &=0;
}

