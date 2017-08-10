////////////////||spi1MOSI	|	PA7 ||//////////////////////////
////////////////||spi1MISO	|	PA6 ||//////////////////////////
////////////////||spi1SCK  	|	PA5 ||//////////////////////////
////////////////||spi1NSS  	|	PA4 ||//////////////////////////



#include "TaskManager.h"
#include "USART.h"
#include "SPI.h"
#include "MFRC522.h"


USART com(1,115200);
SPI spi1(SPI1);
MFRC522 rfid(&spi1);

unsigned char defaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t dataWrite[16]={1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,};

int main()
{
	TaskManager::DelayS(2);
	rfid.PCDReset();
	com<<"init complete\n";
	while(1)
	{
		uint8_t mTagInfo[4];
		if(rfid.PcdRequest(MFRC522_PICC_REQIDL,mTagInfo))
		{
			com<<"find card\n";
			if(((u16)mTagInfo[0]<<8|mTagInfo[1])==MFRC522_PICC_MIFARE_ONE_S50)
			{
				com<<"car type is MI1S50\n";
				if(rfid.PcdAntiColl(mTagInfo))
				{
					com<<"anticoll success!UID:";
					com.PrintHex(mTagInfo,4," ")<<"\n";
					if(rfid.PcdSelect(mTagInfo))
					{	
						com<<"select card success\n";
						if(rfid.PcdAuthState(MFRC522_PICC_AUTHENT1A, 1, defaultKey, mTagInfo))
						{
							com<<"auth success!\n";
							if(rfid.PcdWrite(2,dataWrite))
							{
								com<<"write success:";
								com.PrintHex(dataWrite,16," ")<<"\n";
								uint8_t cardValue[16] = {0};
								if(rfid.PcdRead(2,cardValue))
								{
									com<<"read success:";
									com.PrintHex(cardValue,16," ")<<"\n";
								}
							}
						}
					}
				}
				
			}
		}
	}
	
}

