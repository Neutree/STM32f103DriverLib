#include "flash.h"


flash::flash(uint32_t startAddress,bool useHalfWord)
{
	if(startAddress%MEMORY_PAGE_SIZE !=0)//不是页的开始,将开始处设置为下一个页开始的地方
		startAddress+=(MEMORY_PAGE_SIZE-(startAddress%MEMORY_PAGE_SIZE));
	mStartAddress=startAddress;
	mUseHalfWord=useHalfWord;
}



///////////////////////
///读取储存器中特定位置的值
///@param -pageNumber 相对于开始地址的地址
///@param -Data 读出的数据存放的地址
///@retval -1 : 读取成功 -0：读取失败
///////////////////////
bool flash::Read(u16 pageNumber,u16 position,u16* data,u16 length)
{
	u16 dataLength=length;
	if(mUseHalfWord)
	{
		while(dataLength)
		{
			*data=(*(__IO uint16_t*)(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position  +(length-dataLength)*2));
			++data;
			--dataLength;
		}
	}
	else
	{
		while(dataLength)
		{
			*data=(u32)(*(__IO uint32_t*)(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position  +(length-dataLength)*4));
			++data;
			--dataLength;
		}
	}
	return true;
}
///////////////////////
///读取储存器中特定位置的值
///@param -relativeAddress 相对于开始地址的地址
///@param -Data 读出的数据存放的地址
///@retval -1 : 读取成功 -0：读取失败
///////////////////////
bool flash::Read(uint16_t pageNumber, uint32_t* data,u16 length)
{
	u16 dataLength=length;
	if(mUseHalfWord)
	{
		while(dataLength)
		{
			*data=(u32)(*(__IO uint16_t*)(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+(length-dataLength)*2));
			++data;
			--dataLength;
		}
	}
	else
	{
		while(dataLength)
		{
			*data=(*(__IO uint32_t*)(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+(length-dataLength)*4));
			++data;
			--dataLength;
		}
	}
	return true;
}


///////////////////////
///向储存器中特定位置写值
///@param -pageNumber 相对于开始地址的页地址
///@param -Data 将要写入的数据
///@attention 如果构造构造函数的参数useHalfWord为true时，会现将其转换为u16再储存，否则会转换成u32再储存
///@retval -1 : 写入成功 -0：写入失败
///////////////////////
bool flash::Write(uint16_t pageNumber, uint8_t* data,u16 length)
{
	u16 dataLength=length;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	if(!FLASH_ErasePage(mStartAddress+pageNumber*MEMORY_PAGE_SIZE))//擦除页
		return false;
	if(mUseHalfWord)
	{
		while(dataLength)
		{
			if(FLASH_COMPLETE!=FLASH_ProgramHalfWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+(length-dataLength)*2,(u16)*data))
				return false;
			++data;
			--dataLength;
		}
	}
	else
	{
		while(dataLength)
		{
			if(FLASH_COMPLETE!=FLASH_ProgramWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+(length-dataLength)*4,(u32)*data))
				return false;
			++data;
			--dataLength;
		}
	}
	FLASH_Lock();
	return true;
}
		


		
///////////////////////
///向储存器中特定位置写值
///@param -pageNumber 相对于开始地址的地址
///@param -Data 将要写入的数据
///@retval -1 : 写入成功 -0：写入失败
////
bool flash::Write(uint16_t pageNumber,u16 position,uint16_t* data,u16 length)
{
	if(position+length>MEMORY_PAGE_SIZE)
		return false;
	
	u16 dataLength=length;
	FLASH_Unlock();
	
	if(position==0){
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	if(!FLASH_ErasePage(mStartAddress+pageNumber*MEMORY_PAGE_SIZE))//擦除页
		return false;
	}
	
	if(mUseHalfWord)
	{
		while(dataLength)
		{
			if(FLASH_COMPLETE!=FLASH_ProgramHalfWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position  +(length-dataLength)*2,*data))
				return false;
			++data;
			--dataLength;
		}
	}
	else
	{
		while(dataLength)
		{
			if(FLASH_COMPLETE!=FLASH_ProgramWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position +(length-dataLength)*4,(u32)*data))
				return false;
			++data;
			--dataLength;
		}
	}
	FLASH_Lock();
	return true;
}

		
///////////////////////
///向储存器中特定位置写值
///@param -pageNumber 相对于开始地址的地址
///@param -Data 将要写入的数据
///@attention 如果初始化时选择的是使用办字节，则会将参数强制转换成u16型数据然后再进行储存，可能会出现数据的丢失
///@retval -1 : 写入成功 -0：写入失败
///////////////////////
bool flash::Write(uint16_t pageNumber, uint32_t* data,u16 length)
{
	u16 dataLength=length;
	FLASH_Unlock();	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	if(!FLASH_ErasePage(mStartAddress+pageNumber*MEMORY_PAGE_SIZE))//擦除页
		return false;
	if(mUseHalfWord)
	{
		while(dataLength)
		{
			if(FLASH_COMPLETE!=FLASH_ProgramHalfWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+(length-dataLength)*2,(u16)*data))
				return false;
			++data;
			--dataLength;
		}
	}
	else
	{
		while(dataLength)
		{
			if(FLASH_COMPLETE!=FLASH_ProgramWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+(length-dataLength)*4,*data))
				return false;
			++data;
			--dataLength;
		}
	}
	FLASH_Lock();
	return true;
}


//字符串的保存方式为（字长+字符串）
bool flash::Write(uint16_t pageNumber,u16 position,char* str)
{
	u16 number=0;
	u32 Lenth=0;
	FLASH_Unlock();
	if(position ==0){//只有对该页开始位置写入时查出这也,不允许直接从中间开始写
	FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
					FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
	if(!FLASH_ErasePage(mStartAddress+pageNumber*MEMORY_PAGE_SIZE))//擦除页
				return false;
	}
	
	//计算出字符串长度
	while( (*str)!= '\0')
		{number++;str++;}
	if(number+position<1024)
		FLASH_ProgramHalfWord((mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position)  +Lenth,number); //保存字符串长度
	else
		return false;
		Lenth+=2;
	str=str - number ;
	u16 temp;

		while((*str)!= '\0')
		{
			temp =( (*str)<<8 )+( *(str+1));			
			str+=2;
			FLASH_ProgramHalfWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position  +Lenth,temp);
			Lenth+=2;
			number-=2;
			
			if(number==1)
			{
				temp =*str;	
				FLASH_ProgramHalfWord(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position +Lenth,temp);
				str++;
			}
			
		}
	
	FLASH_Lock();
	return true;
}


//字符串读取
bool  flash::Read(uint16_t pageNumber,u16 position,char *str)
{
	u16 number=0;
	u32 Lenth=0;
	u16 temp;
	number=(*( uint16_t*)(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position  +Lenth));
	if(number + position>1024)
		return false;
	Lenth+=2;
	u16 temp_number = number;
	
		while(number>0)
		{
			temp = (*(__IO uint16_t*)(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position  +Lenth));
			*str = (u8)(temp>>8);
			*(str+1) = (u8)temp;
			Lenth +=2;
			number-=2;
			str+=2;
			
			if(number ==1)//如果是奇数
			{
				temp = (*(__IO uint16_t*)(mStartAddress+pageNumber*MEMORY_PAGE_SIZE+position  +Lenth));
				*(str) = (u8)temp;
				number--;
				str++;
			}
		}
		
		*(str) = '\0' ;
		str= str-temp_number;

		
	return true;

}

//擦除
bool flash::Clear(uint16_t pageNumber)
{
	FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_EOP|
		FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
		if(!FLASH_ErasePage(mStartAddress+pageNumber*MEMORY_PAGE_SIZE))//擦除页
				return false;
		FLASH_Lock();
	return true;
}
