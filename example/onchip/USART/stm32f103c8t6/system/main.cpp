#include "USART.h"

USART usart(1,115200,true,0,3,7,1,3);//
USART usart2(2,115200);//
USART usart3(3,115200);//
u8 dataSend[9]={"abcdefg\n"};
u8 dataReceived[50];
int main()
{
	int i=0;
	usart<<"init complete\n";
	while(1)
	{
		++i;
/*		usart.SendData(dataSend,9);*/
		usart<<i<<"**123\n";
		usart.Setprecision(2)<<12.3<<"\n";
		for(uint32_t i=0;i<100000;++i);
		if(usart.ReceiveBufferSize()>0)
		{
			int receivedDataNum=usart.ReceiveBufferSize();
			usart.GetReceivedData(dataReceived,receivedDataNum);
			usart.SendData(dataReceived,receivedDataNum);
		}

		usart2.SendData(dataSend,9);
		usart2<<"aaaaaa";
		for(uint32_t i=0;i<1000000;++i);
		if(usart2.ReceiveBufferSize()>0)
		{
			int receivedDataNum=usart2.ReceiveBufferSize();
			usart2.GetReceivedData(dataReceived,receivedDataNum);
			usart2.SendData(dataReceived,receivedDataNum);
		}
		
		
		usart3.SendData(dataSend,9);
		for(uint32_t i=0;i<1000000;++i);
		if(usart3.ReceiveBufferSize()>0)
		{
			int receivedDataNum=usart3.ReceiveBufferSize();
			usart3.GetReceivedData(dataReceived,receivedDataNum);
			usart3.SendData(dataReceived,receivedDataNum);
		}
		
	}
	
}

