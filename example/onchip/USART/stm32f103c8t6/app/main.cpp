#include "USART.h"

USART usart(1,115200,false,0,3,7,1,3);//
USART usart2(2,115200);//
USART usart3(3,115200);//
u8 dataSend[9]={"abcdefg\n"};
u8 hexData[5]={0x12,0x56,0x9A,0xAB,0xEF};
u8 dataReceived[50];
int main()
{
	int i=0;
	usart<<"init complete\n";
	while(1)
	{
		++i;
/*		usart.SendData(dataSend,8);*/
		usart<<i<<"**123\n";
		usart.Setprecision(2)<<"\t\t"<<12.3<<"\n";
		usart<<"hex data: ";
		usart.PrintHex(hexData,5,"-")<<"\n";


		for(uint32_t i=0;i<10000;++i);
		if(usart.ReceiveBufferSize()>0)
		{
			int receivedDataNum=usart.ReceiveBufferSize();
			usart.GetReceivedData(dataReceived,receivedDataNum);
			usart.SendData(dataReceived,receivedDataNum);
		}

		usart2.SendData(dataSend,8);
		usart2<<"aaaaaa";
		for(uint32_t i=0;i<100000;++i);
		if(usart2.ReceiveBufferSize()>0)
		{
			int receivedDataNum=usart2.ReceiveBufferSize();
			usart2.GetReceivedData(dataReceived,receivedDataNum);
			usart2.SendData(dataReceived,receivedDataNum);
		}
		usart3.SendData(dataSend,8);
		for(uint32_t i=0;i<100000;++i);
		if(usart3.ReceiveBufferSize()>0)
		{
			int receivedDataNum=usart3.ReceiveBufferSize();
			usart3.GetReceivedData(dataReceived,receivedDataNum);
			usart3.SendData(dataReceived,receivedDataNum);
		}
		
	}
	
}

