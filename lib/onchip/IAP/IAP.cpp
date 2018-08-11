
#include "IAP.h"

IAP::IAP(Flash& flash,uint32_t appAddr )
:mFlash(flash)
{
    mAppAddr = appAddr;
    __enable_irq();
}

bool IAP::LoadApp()
{
    void (*pFun)(void);

    RCC_DeInit();    //关闭外设时钟
    NVIC_DeInit ();  //恢复NVIC为复位状态.使中断不再发生.
    __disable_irq();

	if(((*(uint32_t*)mAppAddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		pFun=(void(*)(void))*(vu32*)(mAppAddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		// MSR_MSP(*(vu32*)mAppAddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
        __set_MSP(*(uint32_t*)mAppAddr);                //设置SP.，堆栈栈顶地址
		pFun();									        //跳转到APP.
		return true;
		
	}else{
		return false;
	}
}


bool IAP::CheckPack(IAP_Pack_t* pack)
{
    uint16_t i;
    uint16_t packLen = sizeof(IAP_Pack_t)-1;
    uint8_t sum = *((uint8_t*)pack);
    for(i=1; i<packLen; ++i)
    {
        sum ^= ((uint8_t*)pack)[i];
    }
    return sum == pack->checkSum;
}

bool IAP::WritePack(IAP_Pack_t* pack, bool checkPack)
{
    uint16_t i=0;
    uint16_t temp;

    if(checkPack && !CheckPack(pack))
        return false;
    if(mFlash.IsHalfWord())
    {
        if(pack->len % 2)
            return false;
        uint16_t packLen = pack->len/2;
        if(!mFlash.Write(pack->id,(uint16_t*)(pack->data),packLen))
            return false;
    }
    else
    {
        if(pack->len % 4)
            return false;
        uint16_t packLen = pack->len/4;
        if(!mFlash.Write(pack->id,(uint32_t*)(pack->data),packLen))
            return false;
    }
    return true;
}

// extern "C"
// {
	
//     __asm void MSR_MSP(u32 addr) 
//     {
//         MSR MSP, r0 			//set Main Stack value
//         BX r14
//     }

// }

