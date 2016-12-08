
#include "yishan_PM2_5.h"

yishan_PM_2_5::yishan_PM_2_5(USART &com):mCom(com)
{
	 PM1_0_UD=0;
	 PM2_5_UD = 0;
	 PM10_UD = 0;
}

bool yishan_PM_2_5::Check(u8 *Data,int Lenth,int CheckSum)
{
	int temp=0;
	for(int i=0;i<Lenth;i++)
	{
		temp+=Data[i];
	}
	
	if(temp == CheckSum)
		return true;
	else
		return false;
}

int yishan_PM_2_5::GetConcentration_2_5()
{
	return PM2_5_UD;
}

int yishan_PM_2_5::GetConcentration_1_0()
{
	return PM1_0_UD;
}

int yishan_PM_2_5::GetConcentration_10()
{
	return PM10_UD;
}

bool yishan_PM_2_5::Update()
{
	u8 data[32];
	u8 ch[2];
	int num=mCom.ReceiveBufferSize();
	if(num > 31)
	{
		mCom.GetReceivedData(ch,2);
		if(ch[0] != 0x32 || ch[1] != 0x3d)
		{
			mCom.ClearReceiveBuffer();
			return false;
		}
//		if(ch!=0x32)
//			return false;
//		else
//		{
//			mCom.GetReceivedData(&ch,1);
//			if(ch!=0x3D)
//				return false;	
//		}
		mCom.GetReceivedData(data+2,30);
		data[0] = 0x32;
		data[1]= 0x3d;
		
		if(Check(data,30,(int)data[30]*256+data[31]))//校验
			{
					//数据处理
					PM1_0_UD = (int)data[4]*256 + data[5];
					PM2_5_UD = (int)data[6]*256 + data[7];
					PM10_UD  = (int)data[8]*256 + data[9];
					mCom.ClearReceiveBuffer();
					return true;
			}
			else
				return false;
	}
	else
		return false;
	
}
