#ifndef __DOOR_H
#define __DOOR_H

#include "GPIO.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdint.h"


class Door{

public:
    Door(GPIO* in, bool normalOpen, GPIO* out = NULL);
	bool Init();
    bool IsOpen();
    bool Update();

private:
    bool mNormalOpen;
    bool mIsOpen;
    GPIO* mGPIOIn;
    GPIO* mGPIOOut;
};

#endif
