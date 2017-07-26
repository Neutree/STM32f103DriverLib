/**
*@file SPIFlash.cpp
*@author 2017-07-26 Necrack
*@version v0.1
*@brief spi flash(W25Qxx) read write      
*      
*      
*@attention , no DIO QIO etc. normal spi communicate. not all test yet,see spi flash example in example folder
*
*/
#include "SPIFlash.h"

SPIFlash::SPIFlash(SPI& spi,GPIO* wp,GPIO* hold)
:mSpi(spi)
{
	if(wp!=0)
		wp->SetLevel(1);
	if(hold!=0)
		hold->SetLevel(1);
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0xff);
	spi.DisableSPI();
}
u16 SPIFlash::GetID(u8 idAddr)
{
	u8 temp[2] = {0};
	bool success=true;
	mSpi.EnableSPI();
	success &= mSpi.ReadOrWriteByte(idAddr);
	success &= mSpi.ReadOrWriteByte(0x00);
	success &= mSpi.ReadOrWriteByte(0x00);
	success &= mSpi.ReadOrWriteByte(0x00);
	success &= mSpi.ReadOrWriteByte(0xff,temp);
	success &= mSpi.ReadOrWriteByte(0xff,temp+1);
	mSpi.DisableSPI();
	if(success)
		return temp[0]<<8|temp[1];
	return 0;
}
u16 SPIFlash::DeviceID()
{
	return GetID(W25X_DeviceID);
}
u16 SPIFlash::ManufactDeviceID()
{
	return GetID(W25X_ManufactDeviceID);
}
u16 SPIFlash::JedecDeviceID()
{
	return GetID(W25X_JedecDeviceID);
}
u16 SPIFlash::UniqueID()
{
	return GetID(W25X_JedecUniqueID);
}

u8 SPIFlash::ReadSR(void)
{
	u8 revt = 0;
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0x05);
	mSpi.ReadOrWriteByte(0xff,&revt);
	mSpi.DisableSPI();
	return revt;
}

bool SPIFlash::WriteSR(u8 sr)
{
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0x01);
	mSpi.ReadOrWriteByte(sr);
	mSpi.DisableSPI();
	return true;
}

bool SPIFlash::WaiteBusy(float timeoutS)
{
	float timeStart = TaskManager::Time();
	
	while(1)
	{
		if(!(ReadSR()&0x01))
			break;
		if(TaskManager::Time()-timeStart>timeoutS)//time out : s
		{
			return false;
		}
	}
	return true;
}

bool SPIFlash::WriteEnable(void)
{
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0x06);
	mSpi.DisableSPI();
	return true;
}

bool SPIFlash::WriteDisable(void)
{
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0x04);
	mSpi.DisableSPI();
	return true;
}

bool SPIFlash::PowerDown(void)
{
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0xB9);
	mSpi.DisableSPI();
	TaskManager::DelayUs(3);
	return true;
}

bool SPIFlash::WakeUp(void)
{
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0xAB);
	mSpi.DisableSPI();
	TaskManager::DelayUs(3);
	return true;
}


/**
	*256 Bytes every page
	*/
bool SPIFlash::WritePage(u32 addr,u8* pBuffer,u16 nBytes)
{
	WriteEnable();
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0x02);
	mSpi.ReadOrWriteByte((addr & 0xFF0000) >> 16);
	mSpi.ReadOrWriteByte((addr & 0xFF00) >> 8);
	mSpi.ReadOrWriteByte(addr & 0xFF);
	for(u16 i=0;i<nBytes;++i)
		mSpi.ReadOrWriteByte(pBuffer[i]);
	mSpi.DisableSPI();
	return WaiteBusy(5);
}

bool SPIFlash::EraseAll(void)
{
	WriteEnable();
	WaiteBusy(10);
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0xC7);
	mSpi.DisableSPI();
	return WaiteBusy(40);
}

/**
	*4k every sector
	*/
bool SPIFlash::EraseSector(u32 sectorAddr)
{
	sectorAddr*=4096;
	WriteEnable();
	WaiteBusy(10);
	mSpi.EnableSPI();
	mSpi.ReadOrWriteByte(0x20);
	mSpi.ReadOrWriteByte((sectorAddr & 0xFF0000) >> 16);
	mSpi.ReadOrWriteByte((sectorAddr & 0xFF00) >> 8);
	mSpi.ReadOrWriteByte(sectorAddr & 0xFF);
	mSpi.DisableSPI();
	return WaiteBusy(10);
}

uint8_t SPI_FLASH_BUF[4096];
bool SPIFlash::Write(u32 addr,u8* pBuffer,u16 nBytes)
{
	
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	
	secpos=addr/4096;//扇区地址 0~511 for w25x16
	secoff=addr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
//	SPI_FLASH_BUF = (uint8_t *)rt_malloc(4096);
//	if(!SPI_FLASH_BUF)
//		return ;
	if(nBytes<=secremain)secremain=nBytes;//不大于4096个字节
	while(1) 
	{	
		Read(secpos*4096,SPI_FLASH_BUF,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(SPI_FLASH_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			EraseSector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				SPI_FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			WriteNoCheck(secpos*4096,SPI_FLASH_BUF,4096);//写入整个扇区  

		}else
			WriteNoCheck(addr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(nBytes==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
				addr+=secremain;//写地址偏移	   
		   	nBytes-=secremain;				//字节数递减
			if(nBytes>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=nBytes;			//下一个扇区可以写完了
		}	 
	}
	return false;
}

bool SPIFlash::Read(u32 addr,u8* pBuffer,u16 nBytes)
{
	uint16_t i; 
	mSpi.EnableSPI();                          //使能器件   
	mSpi.ReadOrWriteByte(W25X_ReadData);         //发送读取命令   
	mSpi.ReadOrWriteByte((addr & 0xFF0000) >> 16);  //发送24bit地址    
	mSpi.ReadOrWriteByte((addr& 0xFF00) >> 8);   
	mSpi.ReadOrWriteByte(addr & 0xFF);   
	for(i=0;i<nBytes;i++)
	{ 
		mSpi.ReadOrWriteByte(0XFF,pBuffer+i);   //循环读数  
	}
	mSpi.DisableSPI();                            //取消片选    
	return true;
}

bool SPIFlash::WriteNoCheck(u32 addr,u8* pBuffer,u16 nBytes)
{
	uint16_t pageremain;	   
	pageremain=256-addr%256; //单页剩余的字节数		 	    
	if(nBytes<=pageremain)
		pageremain=nBytes;//不大于256个字节
	while(1)
	{	   
		WritePage(addr,pBuffer,pageremain);
		if(nBytes==pageremain)
			break;//写入结束了
	 	else //nBytes>pageremain
		{
			pBuffer+=pageremain;
			addr+=pageremain;

			nBytes-=pageremain;			  //减去已经写入了的字节数
			if(nBytes>256)
				pageremain=256; //一次可以写入256个字节
			else
				pageremain=nBytes; 	  //不够256个字节了
		}
	}
	return true;
}


