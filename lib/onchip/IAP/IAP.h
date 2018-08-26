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
	/**
      *
      *@param flash Flash object, can be NULL if you are in APP program but IAP program
      */
    IAP(Flash* flash,uint32_t appAddr);
    bool CheckPack(IAP_Pack_t* pack);    // for IAP
    bool WritePack(IAP_Pack_t* pack, bool checkPack = false);    // for IAP
	/**
      * IAP program load(and run) App program
      */
    bool LoadApp();                      // for IAP
	/**
	  * Initilize app(IAP program or App program), you must call this function
      *         in on the start of IAP program **and** APP program
      * 
	  *@param currAppAdress current App adress, must be the start address of current application(IAP or user APP)
      *         this value **must** be a multiple of 0x200. 
	  */
	void Init(uint32_t currAppAdress); // for IAP and App

private:
    Flash*   mFlash;
    uint32_t mAppAddr;    
};

#endif
