/*
	每秒发送一次，波特率9600 一个包长度为32个字节
*/

#ifndef __YISHAN_PM2_5_H__
#define __YISHAN_PM2_5_H__

#include "stm32f10x.h"
#include "USART.h"

class yishan_PM_2_5{
	
	private:
		USART &mCom;
		int PM1_0_UD;
		int PM2_5_UD;//PM2.5浓度 单位 ug/m^3
		int PM10_UD;

		bool Check(u8 *Data,int Lenth,int CheckSum);
		
	public:
		yishan_PM_2_5(USART &com);
	
		bool Update();
		int GetConcentration_2_5();
		int GetConcentration_1_0();
		int GetConcentration_10();
	
};

#endif

