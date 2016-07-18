/**@file HMI.h
  *@brief HMI串口屏驱动，使其更加方便的使用串口屏来显示数据
  *    使用：
  *   
  *       
  *       
  * 
  *@author lissettecarlr
  *
  */

#ifndef  __HMI_H
#define  __HMI_H

#include "stm32f10x.h"
#include "USART.h"
#include "TaskManager.h"
#include "stdio.h"

class HMI 
{
	private:
		USART &COM;
		
	
	public:
		
 	  HMI(USART &HMI_com);
	
		bool setTextBox(char *ControlsName,char *Content);
		bool setTextBox(char *ControlsName,float val,u8 decimals);
		bool setTextBox(char *ControlsName,int val);
	
	  bool flushPage(char *Page);
		bool vis(char *ControlsName,bool state); //1显示 0隐藏
//	bool SetTextBox(char *name,char *txt);//设置文本框
//	bool SetProgressBar(char *name,char val);//设置进度条
	
//	bool SetTextBox(USART &com,char *name,char *txt);//设置文本框
//	bool SetProgressBar(USART &com,char *name,char *val);//设置进度条
	
	
	/**********************************Personal  function**************************************/
		bool outputDirection(char *ControlsName,u8 direction); //九宫格表示
	
};

#endif

/**********************************Personal  define**************************************/
#define UI_PITCH "t6"
#define UI_ROOL "t7"
#define UI_YAW "t8"
#define UI_THR "t9"
#define UI_RcPower "t21"
#define UI_CopterPower "t22"
#define UI_DirectionL "t19"
#define UI_DirectionR "t20"
#define UI_CoefficientL "t14"
#define UI_CoefficientR "t15"
#define UI_Hight "t11"
#define UI_AliveTime "t13"
#define UI_FlyClock "p0"


