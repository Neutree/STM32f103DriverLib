/**
*@file IWDG.h
*@author Neucrack
*@version v0.1
*@brief independent watch dog
*
*/
#include "stm32f10x.h"

class IWDG_
{
	public:
	IWDG_(float timeoutS);
	void Feed();
};
