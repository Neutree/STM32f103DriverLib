#include "HCHO.h"

HCHO::HCHO(USART &usart):mUsart(usart)
{

}

HCHO::~HCHO()
{

}

//和校验生成函数
unsigned short HCHO::LRC(unsigned char *data,unsigned char len)
{
	unsigned short lrc = 0;
	for(int i = 0;i<len;i++)
		lrc += *(data+i);
	return lrc;
}


//向HCHO sensor发送查询数据命令
bool HCHO::Updata()
{
	unsigned char const command[7] = {0x42,0x4d,0x01,0x00,0x00,0x00,0x90};
	u8 ch=0;
	u16 datatemp=0;
	mUsart.SendData((u8 *)command,7);
	if(mUsart.ReceiveBufferSize()>=10)
	{
		while(mUsart.ReceiveBufferSize()>=10)
		{
			  mUsart.GetReceivedData(&ch,1);
			if(ch == 0x42)
			{
				mUsart.GetReceivedData(&ch,1);
				if(ch==0x4d)
				{
					mUsart.GetReceivedData(mData,8);
					datatemp = (mData[4]<<8) + mData[5]; //返回数据
					switch (mData[3]) //数据当量
					{
						case 0x01:{data = datatemp/1.0;break;}
						case 0x02:{data = datatemp/10.0;break;}
						case 0x03:{data = datatemp/100.0;break;}
						case 0x04:{data = datatemp/1000.0;break;}
					}
					data_h = (u8)data;
					data_l = (u8) ( (data - data_h)*100 );  
					mUsart.ClearReceiveBuffer();
					return true;
				
				}
			}
		}
	}
	return false;
}

float HCHO::GetFloatData()
{
	return data;
}
	
//正常情况下不得超过0.10mg/m3

unsigned char HCHO::Data_Hight_8()
{
	return data_h;
}
unsigned char HCHO::Data_Low_8()
{
	return data_l;
}
