#ifndef __HUMIDITY_H
#define __HUMIDITY_H

class Humidity{

public:
    virtual float GetHumidity() = 0;
    virtual int Update() = 0;
};

#endif
