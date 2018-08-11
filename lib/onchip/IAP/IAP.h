#ifndef __IAP_H
#define __IAP_H

#include "Flash.h"
#include "stdint.h"
#include "stdbool.h"

typedef struct{
    uint16_t id;                      // packet ID (current packet ), start from 0
    uint16_t len;                     // data length of this packet, must MEMORY_PAGE_SIZE except the last one packet
    uint8_t  data[MEMORY_PAGE_SIZE];  //data    
    uint8_t  checkSum;                // ^ every bytes before
}IAP_Pack_t;

class IAP{

public:
    IAP(Flash& flash,uint32_t appAddr);
    bool CheckPack(IAP_Pack_t* pack);    // for IAP
    bool WritePack(IAP_Pack_t* pack, bool checkPack = false);    // for IAP
    bool LoadApp();                      // for IAP

private:
    Flash&   mFlash;
    uint32_t mAppAddr;
    
};

#endif
