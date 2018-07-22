

#include "Relay.h"
#include "Assert.h"


Relay::Relay(bool highLevelOn)
{
    mHighLevelOn = highLevelOn;
}

bool Relay::Init(GPIO* gpio)
{
	mGPIO = gpio;
	assert(mGPIO != NULL);
	return true;
}

bool Relay::IsOn()
{
    assert(mGPIO != NULL);
    uint8_t status;
    status = mGPIO->GetLevel();
    if(mHighLevelOn)
        return status;
    else
        return status?0:1;
}


void Relay::Set(bool isOn)
{
    assert(mGPIO != NULL);
    uint8_t out;
    if(mHighLevelOn)
        out = isOn?1:0;
    else
        out = isOn?0:1;
    mGPIO->SetLevel(out);
}


