#ifndef _TASK_MANAGER_H_
#define _TASK_MANAGER_H_
#include "stm32f10x.h"

class TaskManager
{
private:
	static double _new_time;		 //current updated time
	static double _old_time;     //last updated time
public:
	static double _it_time;      //time = SysTick interrupt counter*1.8s
public:		
	TaskManager();               //constructor
	static double Time(void);    //get current time
	static void DelayUs(u16 nus);
	static void DelayMs(u16 nms);
	static void DelayS(u16 ns);
	static bool TimeSlice(double &record,double timeout);  //传入一个时间记录值和一个超时时间 记录达到了超时时间返回tree
};
extern TaskManager tskmgr;

#define	MOD_ERROR  0x00
#define	MOD_READY  0x01
#define	MOD_BUSY   0x02
#define	MOD_LOCK   0x04
#define	MOD_UNLOCK 0x08


#endif
