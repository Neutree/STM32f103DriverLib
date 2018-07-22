#ifndef __RELAY_H
#define __RELAY_H

#include "GPIO.h"
#include "stdint.h"
#include "stdbool.h"
#include "stdio.h"

class  Relay{

public:
    Relay(bool HighLevelOn = true);
    bool Init(GPIO* gpio);
    bool IsOn();
    void Set(bool isOn);

private:
    bool mHighLevelOn;
    GPIO* mGPIO;
};


#endif

