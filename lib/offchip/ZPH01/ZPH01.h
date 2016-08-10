/******************************************************************* 
 *  filename:ZPH01.h
 *  brief: 这是一个粉尘传感器的驱动，传感器的型号是ZPH01.可以直接得到一个float的数据值
 *  单位是ug/m3,也可以得到两个8位数据，高位+低位/100=浓度，方便数据成帧传输.参考浓度指标
 *  0-40   优   40-80  良    80-120 中
 *	控制管脚需接地,串口波特率:9600 每秒发送一次
 *  version:1.1
 *  author: lissettecarlr
 *  data: 2016/5/27
 *  note: 一共三个文件 .H .CPP 和一个抽象类:Senser.h组成 
 ******************************************************************/  

#ifndef _ZPH01_H
#define _ZPH01_H

#include <stm32f10x.h>
#include "USART.h"
#include "Senser.h"

class ZPH01:Senser
{
	private:
	
		USART& mUsart;
		
		unsigned char mData[9];	//一帧数据大小
	
		float Concentration;//保存浓度值
	
		unsigned char FucCheckSum(unsigned char *i,unsigned char ln);//校验
	public:
	
		ZPH01(USART &usart);
		
		~ZPH01();
	
	  u8 data_h;//保存浓度半分比的整数部分
		u8 data_l;//保存小数部分
	
		virtual bool  Updata();		//更新
		virtual float GetFloatData();	//返回浓度值 单位ug/m3
		virtual unsigned char Data_Hight_8(); 
		virtual unsigned char Data_Low_8();
	
};


#endif


