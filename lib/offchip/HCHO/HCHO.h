/******************************************************************* 
 *  filename:HCHO.h
 *  brief: 这是一个甲烷传感器的驱动，传感器的型号是DS-HCHO.可以直接得到一个float的数据值
 *  单位是mg/m3,也可以得到两个8位数据，高位+低位/100=浓度，方便数据成帧传输
 *  version:1.2
 *  author: lissettecarlr
 *  data: 2016/5/27
 *  note: 一共三个文件 .H .CPP 和一个抽象类:Senser.h组成 
 ******************************************************************/  

#ifndef _HCHO_H
#define _HCHO_H

#include <stm32f10x.h>
#include "USART.h"
#include "Senser.h"

#define HCHO_DATASIZE 20

#define _HCHO 0x14

#define PPM 	0x01
#define VOL 	0x02
#define LEL 	0x03
#define PPB 	0x04
#define MGM3 	0x05



class HCHO:public Senser
{
	private:
		
		USART &mUsart;
		float data;
		unsigned char mData[HCHO_DATASIZE];			//数据区
		/**
		*@brief 和校验函数（只是简单的将数据求和）
		*@param data :要生成和校验的数据的首地址
		*/
		unsigned short LRC(unsigned char *data,unsigned char len);
	
	public:
			
		u8 data_h;
	  u8 data_l;
	
		HCHO(USART &usart);
		~HCHO();
	
		virtual bool Updata();
	
		/**
		*@brief  得到甲醛浓度的数据
		*@retval 甲醛浓度的高低字节数据
		*/
		virtual	float GetFloatData();		
		virtual unsigned char Data_Hight_8(); 
		virtual unsigned char Data_Low_8();
};


#endif
