#include "ZPH01.h"

ZPH01::ZPH01(USART &usart):mUsart(usart)
{

}

ZPH01::~ZPH01()
{
}


bool ZPH01::Updata()
{
	int num=mUsart.ReceiveBufferSize();
	if(num%9!=0 || num>=90 ||num<9)
	{
		mUsart.ClearReceiveBuffer();
		return false;
	}
	else
	{
		for(int i=0;i<num/9;i++) //取最新一组数据
			mUsart.GetReceivedData(mData,9); 
		if(FucCheckSum(mData,8)==mData[8])
		{
//			data_h=mData[3];
//			data_l=mData[4];
			Concentration = (mData[3] + mData[4] /100.0)*20;	
			data_h=(u8)Concentration; //保存整数
			data_l=(u8)  ( (u16)(Concentration*10)%10 );  //保存小数
			mUsart.ClearReceiveBuffer();
			return true;
		}
		else
		{
			mUsart.ClearReceiveBuffer();
			return false;
		}
	}
}

//校验
unsigned char ZPH01::FucCheckSum(unsigned char *i,unsigned char ln)
{
	unsigned char j,tempq=0;
	i+=1;
	for(j=0;j<(ln-2);j++)
	{
		tempq+=*i;
		i++;
	}
		tempq=(~tempq)+1;
		return(tempq);
}

//获得数据
float ZPH01::GetFloatData()
{
	return Concentration;
}		

unsigned char ZPH01::Data_Hight_8()
{
	return data_h;
}
unsigned char ZPH01::Data_Low_8()
{
	return data_l;
}

