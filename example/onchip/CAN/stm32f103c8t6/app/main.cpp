#include "USART.h"
#include "can.h"
#include "TaskManager.h"

USART usart(1,115200,false);

CAN c(0);

int main()
{
	 usart<<"init complete\n";
	double a=0;
	 u8 data[8]= {1,2,3,4,5,6,7,8};
	while(1)
	{
		 if(tskmgr.TimeSlice(a,1))
     {
			  //串口接收
		    if(usart.ReceiveBufferSize()>=8)
				{  
				// u16 len1;
				// len1 = usart.ReceiveBufferSize();
					 usart.GetReceivedData(data,8);
				   c.Can_Send_Msg(data,8);						
					 usart <<"CAN Send : .\n";
				}
				//CAN接收
				if(c.ReceivedBufferSize() >= 1)
				{
				   u8 data2[100]; 
					 u16 len2 = c.ReceivedBufferSize();
					 if(len2 >=8)
						c.GetReceivedData(data2,8);
           else					 
					  c.GetReceivedData(data2,len2);
					 
           usart<<"CAN recive: \n";					 
					 if(len2 >=8)
						usart.SendData(data2,8);
           else					 
					  usart.SendData(data2,len2);
					  
					 
				}
		 }
	}
	
}

