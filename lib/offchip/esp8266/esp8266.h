#ifndef __ESP8266_H
#define __ESP8266_H


/**
  *@file esp8266.h
  *@brief The definition of class esp8266 and related operation of esp8266 module
  *@author neucrack(neucrack.com)  CQUT IOT lib
  *@date 2015-11
  *@copyright CQUT IOT lib all right reserved
  */



#include "USART.h"
#include "string.h"
#include "TaskManager.h"
#include "socket.h"
/*********************configuration****************************/
		#define ESP8266_RECEIVE_BUFFER_SIZE  50
		#define ESP8266_DEFAULT_PATTERN  esp8266_PATTERN_DEF
		#define ESP8266_DEFAULT_TIMEOUT  5  //单位：s

/**************************************************************/


class esp8266
{
	public:
	enum esp8266_pattern{esp8266_PATTERN_NULL=0,esp8266_PATTERN_CUR=1,esp8266_PATTERN_DEF=2};
	enum esp8266_MODE{esp8266_MODE_STATION=1,esp8266_MODE_AP=2,esp8266_MODE_STATION_AP=3};
	enum esp8266_ENCRYPTION{esp8266_ENCRYPTION_OPEN=0,esp8266_ENCRYPTION_WEP=1,
		esp8266_ENCRYPTION_WPA_PSK=2,esp8266_ENCRYPTION_WPA2_PSK=3,esp8266_ENCRYPTION_WPA_WPA2_PSK=4};
	enum esp8266_ROLE{esp8266_ROLE_SERVER=1,esp8266_ROLE_CLIENT=2};
	/**
	 * Constuctor. 
	 *
	 * @param uart - an reference of HardwareSerial object. 
	 * @param baudRate - the baud rate to communicate with ESP8266 when first power on (default:the same as usart's baud rate). 
	 *              if the value is not the same as usart's baud rate, it will change usart's baud rate to the same as thid parameter's value
	 * @warning parameter baud depends on the AT firmware. 9600 is an common value. 
	 */
	esp8266(USART &usart, uint32_t baudRate = 0 );
	
	void ClearBuffer();
	
    bool Kick(void);
    bool Restart(void);
    bool GetVersion(char*);
    bool SetEcho(bool echo);
    bool Restore(void);
    bool SetUart(uint32_t baudrate,esp8266_pattern pattern);

	bool SetMode(esp8266_MODE,esp8266_pattern = esp8266_PATTERN_DEF);//设置为station+ap模式
	bool SetMUX(bool isEnableMUX);
	bool SetApParam(char* apSetSsid,char* apSetPasswd,unsigned char channel=1,esp8266_ENCRYPTION encryption=esp8266_ENCRYPTION_WPA2_PSK,esp8266_pattern = esp8266_PATTERN_DEF);
	bool JoinAP(char* apJoinSsid,char* apJoinPasswd,esp8266_pattern = esp8266_PATTERN_DEF);//加入AP
	
	bool CreateConnectMutipleMode(char* ipAddr,short port,Socket_Type socketType, signed char muxID = -1);
	bool SendMultipleMode(char* data, unsigned int num,signed char muxID=-1);
	bool Close(signed char muxID=-1);
	char GetStatus(char* muxID=0,char* type=0,char* ipAddr=0,short remotePort=0,short localPort=0,esp8266_ROLE role=esp8266_ROLE_CLIENT);
	
private:
		
	USART &mUsart;
	unsigned int mReceiveBufferIndex;

	bool ReceiveAndWait(char const* targetString,unsigned char timeOut=ESP8266_DEFAULT_TIMEOUT);
	bool ReceiveAndWait(char const* targetString,char const* targetString2,unsigned char timeOut=ESP8266_DEFAULT_TIMEOUT);
	bool ReceiveAndWait(char const* targetString,char const* targetString2,char const* targetString3,unsigned char timeOut=ESP8266_DEFAULT_TIMEOUT);

protected:
	char mReceiveBuffer[ESP8266_RECEIVE_BUFFER_SIZE];
public:
	bool RecvFind(char const *target, unsigned char timeout=ESP8266_DEFAULT_TIMEOUT);
	bool RecvFind(char const *target,char const *target2, unsigned char timeout=ESP8266_DEFAULT_TIMEOUT);
	bool RecvFind(char const *target,char const *target2,char const *target3, unsigned char timeout=ESP8266_DEFAULT_TIMEOUT);
	bool RecvFindAndFilter(char const *target, char const * begin, char const * end, char* Data, float timeout=ESP8266_DEFAULT_TIMEOUT);

	
};
#endif

