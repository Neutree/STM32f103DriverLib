/**
*@file IWDG.cpp
*@author Neucrack
*@version v0.1
*@brief independent watch dog
*
*/
#include "IWDG.h"

IWDG_::IWDG_(float timeoutS)
{
#ifdef  WATCHDOG
    uint8_t prer = 4;
    uint16_t rlr = (uint16_t)timeoutS * 625;
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); /* 使能对寄存器IWDG_PR和IWDG_RLR的写操作*/
    IWDG_SetPrescaler(prer);    /*设置IWDG预分频值:设置IWDG预分频值*/
    IWDG_SetReload(rlr);     /*设置IWDG重装载值*/
    IWDG_ReloadCounter();    /*按照IWDG重装载寄存器的值重装载IWDG计数器*/
    IWDG_Enable();        /*使能IWDG*/
#endif
}

void IWDG_::Feed()
{
	IWDG_ReloadCounter();
}

