#ifndef __SHARP_PM2_5_H__
#define __SHARP_PM2_5_H__

#include "stm32f10x.h"
#include "USART.h"

//波特率 2400  没10ms发送一组数据



class Sharp_PM_2_5{
	
	private:
		USART &mCom;
		float UD;//粉尘浓度 单位 ug/m^3
	  bool Check(u8 *Data,int Lenth,u8 CheckSum);
		
	public:
		Sharp_PM_2_5(USART &com);
		bool Upudata();
		float GetConcentration();
	
};


#define  COEFFICIENT_A 800

#endif
