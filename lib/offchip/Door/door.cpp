

#include "door.h"
#include "Assert.h"


Door::Door(GPIO* in, bool normalOpen , GPIO* out)
{
    assert(in != NULL);
    mNormalOpen = normalOpen;
    mIsOpen = false;
    mGPIOIn = in;
    mGPIOOut = out;
}


bool Door::Init()
{
    if(mGPIOOut)
    {
        mGPIOOut->SetLevel(1);
    }
	return true;
}

bool Door::Update()
{
    uint8_t status = mGPIOIn->GetLevel();
    if(mNormalOpen)
    {
        mIsOpen = (status==1)?false:true;
    }
    else
    {
        mIsOpen = (status==1)?true:false;
    }
	return true;
}


bool Door::IsOpen()
{
    return mIsOpen;
}

