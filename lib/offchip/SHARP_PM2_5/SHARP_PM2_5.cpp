#include "SHARP_PM2_5.h"


Sharp_PM_2_5::Sharp_PM_2_5(USART &com):mCom(com)
{}
	
bool Sharp_PM_2_5::Upudata()
{
	u8 buffer[70]; //buffer 最大70
	u8 data[5];
	float temp;
	
	int num=mCom.ReceiveBufferSize();
	if(num > 7)
	{
		mCom.GetReceivedData(buffer,num);
		while( buffer[--num] !=0xff);
		num = num -6;
		if(buffer[num] == 0xaa)
		{
			for(u8 i=0;i<5;i++)
				data[i] = buffer[num+i+1];
			
			if(Check(data,4,data[4]))//校验
			{
					temp=(float)(data[0]*256+data[1])/1024*5;
					UD=temp * COEFFICIENT_A;
					return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
	
}
	
float Sharp_PM_2_5::GetConcentration()
{
	 return UD;
}
	
bool Sharp_PM_2_5::Check(u8 *Data,int Lenth,u8 CheckSum)
{
	u8 temp =0;
	for(int i=0;i<Lenth;i++)
	{
		temp+=Data[i];
	}
	if(temp == CheckSum)
		return true;
	else
		return false;
	
}
