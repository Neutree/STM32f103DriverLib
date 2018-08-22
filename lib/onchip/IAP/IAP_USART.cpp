#include "IAP_USART.h"

IAP_USART::IAP_USART(USART& usart,Flash& flash,uint32_t appAddr)
:IAP(flash,appAddr),mUsart(usart)
{
    mPackIdLast = 0;
}

IAP_USART_Upgrade_Status_t IAP_USART::Upgrade()
{
    uint32_t startFlag = IAP_USART_PACK_START_FLAG;
    uint8_t temp;
    static uint8_t headerReadLen = 0;
    static uint8_t errorCount = 0;
    IAP_USART_Upgrade_Status_t status = IAP_USART_UPGRADE_STATUS_IDLE;

    while(mUsart.ReceiveBufferSize()+headerReadLen >= sizeof(IAP_USART_Pack_t))
    {
        if(headerReadLen == 4)//find header
        {
			headerReadLen = 0;
            mPack.packStart = startFlag;
            mUsart.GetReceivedData(((uint8_t*)&mPack)+4,sizeof(IAP_USART_Pack_t)-4);
            if(!CheckPack(&mPack))
            {
                if(++errorCount > 3)
                {
                    mPackIdLast = 0;
                    PackAck(mPack.packID,mPack.packTotal,false);
                    return IAP_USART_UPGRADE_STATUS_ERROR;
                }
            }
            if((mPack.packID == 0) || (mPack.packID == mPackIdLast+1))//received the next pack
            {
                if(!WritePack( (IAP_Pack_t*)&mPack.packID, false))
                {
                    if(++errorCount > 3)
                    {
                        mPackIdLast = 0;
                        return IAP_USART_UPGRADE_STATUS_ERROR;
                    }
                    PackAck(mPack.packID,mPack.packTotal,false);
                }
                else
                {
                    if((mPack.packID+1) == mPack.packTotal)//last
                    {
                        mPackIdLast = 0;
                        PackAck(mPack.packID,mPack.packTotal,true);
                        // LoadApp();// upgrade complete, start app now
                        return IAP_USART_UPGRADE_STATUS_COMPLETE;
                    }
                    else
                    {
                        PackAck(mPack.packID,mPack.packTotal,true);
                        mPackIdLast += 1;
                    }
                    
                }
                status = IAP_USART_UPGRADE_STATUS_WORKING;
            }
            else//received the pack received before, just ack pack
            {
                PackAck(mPackIdLast,mPack.packTotal,true);
            }
            break;
        }
        mUsart.GetReceivedData(&temp,1);
        if( temp == ((uint8_t*)&startFlag)[headerReadLen] )
        {
            ++headerReadLen;
        }
        else
        {
            headerReadLen = 0;
        }
        

    }
    if(mPackIdLast != 0)
        status = IAP_USART_UPGRADE_STATUS_WORKING;
    return status;
}


bool IAP_USART::CheckPack(IAP_USART_Pack_t* pack)
{
    uint16_t i;
    uint16_t packLen = sizeof(IAP_USART_Pack_t)/2-1;
    uint16_t sum = *((uint16_t*)pack);
    for(i=1; i<packLen; ++i)
    {
        sum ^= ((uint16_t*)pack)[i];
    }
	if(sizeof(IAP_USART_Pack_t)%2)
	{
		sum ^= ((uint16_t*)pack)[sizeof(IAP_USART_Pack_t)-3];
	}
    return sum == pack->checkSum;
}

bool IAP_USART::AckCheckSum(IAP_USART_Pack_Ack_t* pack)
{
    uint16_t i;
    uint16_t packLen = sizeof(IAP_USART_Pack_Ack_t)/2-1;
    uint16_t sum = *((uint16_t*)pack);
    for(i=1; i<packLen; ++i)
    {
        sum ^= ((uint16_t*)pack)[i];
    }
	if(sizeof(IAP_USART_Pack_Ack_t)%2)
	{
		sum ^= ((uint8_t*)pack)[(sizeof(IAP_USART_Pack_Ack_t)-3)];
	}
	pack->checkSum = sum;
    return true;
}

bool IAP_USART::PackAck(uint16_t packID, uint16_t packTotal, bool success)
{
	IAP_USART_Pack_Ack_t ack;
	ack.packStart = IAP_USART_PACK_ACK_START_FLAG;
	ack.packID = packID;
	ack.packTotal = packTotal;
	ack.success = success;
	AckCheckSum(&ack);
	mUsart.SendData((uint8_t*)&ack,sizeof(IAP_USART_Pack_Ack_t));
	return true;
}




