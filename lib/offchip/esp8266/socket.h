#ifndef __SOCKET_H
#define __SOCKET_H
#include "stdbool.h"

enum Socket_Type{Socket_Type_Stream,Socket_Type_Dgram};//TCP & UDP
enum Socket_Protocol{Socket_Protocol_IPV4,Socket_Protocol_IPV6};
	
class Socket
{
public :
	Socket(){}
	virtual bool Init() = 0;
	virtual bool Connect(char* ipAddr,short int port,Socket_Type socketType,Socket_Protocol socketProtocol=Socket_Protocol_IPV4) = 0;
	virtual bool Write(char* data,unsigned int num) = 0;
	virtual unsigned int Read(char* data) = 0;
	virtual unsigned int Read(char* data,unsigned int num) = 0;
	virtual bool IsAlive() = 0;
	virtual bool Close() = 0;
	virtual void SetTimeOut(float timetOut) = 0;
private:
	long int mIPAddr;
	short int mPort;
	
};

#endif


