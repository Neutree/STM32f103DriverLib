/**
*@file SPIFlash.h
*@author 2017-07-26 Necrack
*@version v0.1
*@brief spi flash(W25Qxx) read write, no DIO QIO etc. normal spi communicate      
*      
*      
*@attention not all test yet,see spi flash example in example folder
*
*/

#ifndef _SPI_FLSH_
#define _SPI_FLSH_

#include "SPI.h"
#include "GPIO.h"

#define W25X_WriteEnable		  0x06 
#define W25X_WriteDisable	   	0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData	    0x0B 
#define W25X_FastReadDual	    0x3B 
#define W25X_PageProgram	    0x02 
#define W25X_BlockErase		    0xD8 
#define W25X_SectorErase	    0x20 
#define W25X_ChipErase		    0xC7 
#define W25X_PowerDown		    0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID		     	0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F
#define W25X_JedecUniqueID		0x4B

enum SPIFLASH_MODE{DIO,QIO,QOUT,DOUT};


class SPIFlash
{
	public:
		SPIFlash(SPI& spi,GPIO* wp=0,GPIO* hold=0);
		u16 DeviceID();
		u16 ManufactDeviceID();
		u16 JedecDeviceID();
		u16 UniqueID();
		u8 ReadSR(void);
		bool WriteSR(u8 sr);
		bool WaiteBusy(float timeoutS);
		bool WriteEnable(void);
		bool WriteDisable(void);
		bool PowerDown(void);
		bool WakeUp(void);
		bool Write(u32 addr,u8* pBuffer,u16 nBytes);
		bool Read(u32 addr,u8* pBuffer,u16 nBytes);
		bool EraseAll(void);
		/**
		  *4k every sector
		  */
		bool EraseSector(u32 sectorAddr);
		/**
		  *unsafe
		  *256 Bytes every page
		  */
		bool WritePage(u32 addr,u8* pBuffer,u16 nBytes);
		bool WriteNoCheck(u32 addr,u8* pBuffer,u16 nBytes);

	private:
		SPI& mSpi;
		SPIFLASH_MODE mMode;
		u16 GetID(u8 idAddr);
};

#endif
