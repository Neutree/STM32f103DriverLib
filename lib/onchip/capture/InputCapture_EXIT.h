#ifndef _INPUT_CAPTURE_EXIT_H_
#define _INPUT_CAPTURE_EXIT_H_

#include "stm32f10x.h"

class InputCapture_EXIT
{
protected:
	GPIO_TypeDef *mExitPort;  //GPIO Port of exit pin
	u16 mExitPin;             //GPIO pin for exit
	float mRiseTime;          //rising edge time
	float mFallTime;          //falling edge time
	float mInterval;          //time between rising edge and falling edge
public:
	InputCapture_EXIT(GPIO_TypeDef *exitPort, u8 exitPin, u8 itGroup=3,u8 prePriority=5,u8 subPriority=0);
	void IRQ();
	float Value();
};





#endif
