#include "Socket_esp8266.h"

Socket_esp8266::Socket_esp8266(USART& usart)
:esp8266(usart),mUsart(usart),mIsConnected(false)
{
	mTimeOut = 3;
	
}
bool Socket_esp8266::Init()
{
	mUsart.ClearReceiveBuffer();//wifi上电后会有一串调试信息，为了避免缓冲区溢出，所以需要在wifi模块上电后几秒钟后清除缓冲区
								//建议上电大于2秒后调用
	return true;
}
bool Socket_esp8266::Connect(char* ipAddr,short port,Socket_Type socketType,Socket_Protocol socketProtocol)
{
	if(CreateConnectMutipleMode(ipAddr,port,socketType))
		mIsConnected = true;
	else
		mIsConnected = false;
	return mIsConnected;
}
bool Socket_esp8266::Write(char* data,unsigned int num)
{
	return SendMultipleMode(data,num);
}
unsigned int Socket_esp8266::Read(char* data)
{
	float starttime = TaskManager::Time();
	if(!mIsConnected)
		return 0;
	unsigned char temp[13];
	unsigned short bufferSize=0;
	unsigned short dataLength=0;
	short count=0;
	uint8_t headFlag=0;
	//读出数据长度
	while(TaskManager::Time()-starttime<mTimeOut){
		bufferSize = mUsart.ReceiveBufferSize();
		if(bufferSize>0)
		{
			mUsart.GetReceivedData(temp,1);
			if(temp[0] == '+')
				headFlag = 1;
			else if(temp[0] == 'I' && headFlag == 1)
				headFlag = 2;
			else if(temp[0] == 'P' && headFlag == 2)
				headFlag = 3;
			else if(temp[0] == 'D' && headFlag == 3)
				headFlag = 4;
			else if(temp[0] == ',' && headFlag == 4)
			{
				if(mIsEnableMUX)
					mUsart.GetReceivedData(temp,2);
				headFlag = 5;
			}
			else if(headFlag == 5)
			{
				if(temp[0] == ':')
				{
					break;
				}
				else
					dataLength = dataLength*10+(temp[0]-'0');
			}
		}
	}
	count=0;
	//读取数据
	while(TaskManager::Time()-starttime<mTimeOut){
		if(mUsart.ReceiveBufferSize()>0)
		{
			mUsart.GetReceivedData((unsigned char*)(data+count),1);
			++count;
			if(count==dataLength || count==ESP8266_RECEIVE_BUFFER_SIZE)
				break;
		}
	}
	return count;
}
unsigned int Socket_esp8266::Read(char* data,unsigned int num)
{
	float starttime = TaskManager::Time();
	if(!mIsConnected)
		return 0;
	unsigned int count=0;
	while(TaskManager::Time()-starttime<mTimeOut&&count<num){
		if(mUsart.ReceiveBufferSize()>0)
		{
			mUsart.GetReceivedData((unsigned char*)(data+count),1);
			++count;
		}
	}
	return count;
}
bool Socket_esp8266::IsAlive()
{
	if(!mIsConnected)
		return false;
	if(GetStatus()==3)
		return true;
	return false;
}
bool Socket_esp8266::Close()
{
	if(mIsConnected)
	{
		esp8266::Close();
	}
	return true;	
}

void Socket_esp8266::SetTimeOut(float timetOut)
{
	mTimeOut = timetOut;
}

bool Socket_esp8266::SetMUX(bool isEnableMUX)
{
	mIsEnableMUX = isEnableMUX;
	return esp8266::SetMUX(isEnableMUX);
}

