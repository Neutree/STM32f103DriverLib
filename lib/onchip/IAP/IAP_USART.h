#ifndef __IAP_USART_H
#define __IAP_USART_H

#include "IAP.h"
#include "USART.h"


/////////////////////////////////////////////////////
/// Attention, the buffer length of USART MUST more than `sizeof(IAP_USART_Pack_t)` (1k+12)
/// setting in Configuration.h
/////////////////////////////////////////////////////


#define IAP_USART_PACK_START_FLAG  0xAE003D72       //little edian
#define IAP_USART_PACK_ACK_START_FLAG  0xAE003E78   //little edian

typedef struct{
    uint32_t packStart;
    uint16_t packTotal;
    uint16_t packID;                 // start from 0
    uint16_t len;
    uint8_t  data[MEMORY_PAGE_SIZE]; //fixed length MEMORY_PAGE_SIZE, if less than MEMORY_PAGE_SIZE, padding with 0
    uint16_t checkSum;    //^ all bytes before
}IAP_USART_Pack_t;  //little edian

typedef enum{
    IAP_USART_UPGRADE_STATUS_ERROR    = -1,
    IAP_USART_UPGRADE_STATUS_IDLE     =  0,
    IAP_USART_UPGRADE_STATUS_WORKING  =  1,
    IAP_USART_UPGRADE_STATUS_COMPLETE =  2,

    IAP_USART_UPGRADE_STATUS_MAX
}IAP_USART_Upgrade_Status_t;

typedef struct
{
    uint32_t packStart;
    uint16_t packID;
    uint16_t packTotal;
    bool     success;
    uint16_t checkSum;
}IAP_USART_Pack_Ack_t;




class IAP_USART:public IAP{

public:
    IAP_USART(USART& usart);
    IAP_USART_Upgrade_Status_t Upgrade();

private:
    USART& mUsart;
    uint16_t mPackIdLast;
    IAP_USART_Pack_t mPack;
    IAP_USART_Pack_Ack_t mPackAck;
    bool CheckPack(IAP_USART_Pack_t* pack);

};

#endif
