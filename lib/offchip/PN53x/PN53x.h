/**
*@file PN53x.h
*@author Neucrack
*@version v0.1
*@brief RFID reader PN53x driver
*@date create:2017.5.29
*      update:2017.5.29
*
*/


#include "USART.h"
//extern USART LOG;

#define MAX_WAIT_TIME_OUT  0.8  //s

class PN53x{

private:
	USART& mUsart;

public:
	PN53x(USART& com);
	bool PcdWakeUp();
	bool  PcdListPassiveTarget(uint16_t* type, uint8_t* uid,uint8_t* sak=0,uint8_t* ats=0);
	bool  PcdVerifyKeyA(uint8_t* uid,uint8_t* key);
	bool  PcdRead(uint8_t* dataRead,uint8_t blockNumber = 2);
	bool  PcdWrite(uint8_t* dataWrite,uint8_t blockNumber = 2);
};
