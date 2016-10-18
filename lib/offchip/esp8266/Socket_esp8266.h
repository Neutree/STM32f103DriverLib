#ifndef __SOCKET_ESP8266_H
#define __SOCKET_ESP8266_H
#include "socket.h"
#include "esp8266.h"
#include "string.h"
#include "stdlib.h"
#include "USART.h"

class Socket_esp8266 :public esp8266,public Socket
{
public:
	Socket_esp8266(USART& usart);
	virtual bool Init();
	virtual bool Connect(char* ipAddr,short port,Socket_Type socketType,Socket_Protocol socketProtocol=Socket_Protocol_IPV4);
	virtual bool Write(char* data,unsigned int num);
	virtual unsigned int Read(char* data);
	virtual unsigned int Read(char* data,unsigned int num);
	virtual bool IsAlive();
	virtual bool Close();
	virtual void SetTimeOut(float timetOut);

	bool SetMUX(bool isEnableMUX);
private:
	USART& mUsart;
	float mTimeOut;
	bool mIsConnected;
	bool mIsEnableMUX;
};

#endif

