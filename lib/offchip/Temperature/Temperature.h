#ifndef __TEMPERATURE_H
#define __TEMPERATURE_H

#include "stdbool.h"
#include "stdint.h"

class Temperature{

public:
    virtual float GetTemperature() = 0;
    virtual int Update() = 0;
};


#endif
