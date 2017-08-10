#include "stm32f10x.h"
#include "Configuration.h"
#include "TaskManager.h"
#include "USART.h"
#include "LED.h"
#include "SPI.h"
#include "SPIFlash.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

USART com(1,115200,false);

GPIO ledRedGPIO(GPIOC,13,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);//LED GPIO
LED ledRed(ledRedGPIO);//LED red
GPIO flashPower(GPIOB,0,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
GPIO flashPowerGnd(GPIOB,1,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);
//GPIO flashWp(GPIOB,14);
//GPIO flashHold(GPIOB,15);
SPI spi(SPI1);
SPIFlash spiFlash(spi);
/**************************************************************************/
/*========================================================================*/
/**************************************************************************/
u8 dataToWrite[256]={0},dataRead[256]={0};
u8 spiFlashSizeM = 4;//4M
u8 dataBuffer[256]={0};
u32 testTimes[6]={0};

void TestWriteReadAll(void);
void TestRandomWriteRead1kTimes(void);
void TestFirstSectorEraseWrite100kTimes(void);
void TestPowerCut1kTimesEvery1Hz10Hz100Hz(void);
bool CanWriteRead(void);
void PowerOff(void);
void PowerOn(void);
bool IsPowerOn(void);

int main()
{
	ledRed.On();
	com<<"\n\n\nSend test times to start program, Use spaces to separate and end with '\\r\\n'\n[erase full chip?(0,1)] [random times(>=0)] [power cut times(>=0)] [erase write times(>=0)] [test all times>=0] [flash size(>0MBytes)] \n\
eg:1 10 10 10 2 4\r\n";
	u8 index = 0;
	while(1)
	{
		while(com.ReceiveBufferSize()>0)			
		{
			com.GetReceivedData(dataBuffer+(index++),1);
			if(index == 255)
			{
				index = 1;
				dataBuffer[0]='\r';
				dataBuffer[1]='\n';
			}
			com.SendData(dataBuffer+index-1,1);
		}
		if(dataBuffer[index-1]=='\n')
		{
			com<<"\nscanf end\n";
			if(index <7 )
			{
				com<<"format error\ntry again\n";
				index =0;
				com.ClearReceiveBuffer();
				continue;
			}
			else
			{
				sscanf((const char*)dataBuffer,"%d %d %d %d %d %d\r\n",testTimes+3,testTimes,testTimes+1,testTimes+2,testTimes+4,testTimes+5);
				break;
			}
		}
	}
	com<<"erase full chip? : "<<(int)(bool(testTimes[3]));
	com<<",random times: "<<(int)testTimes[0]<<", powercut times: "<<(int)testTimes[1]<<", erase write times: "<<(int)testTimes[2]<<",loop times:"<<(int)testTimes[4]<<"flash size:"<<(int)testTimes[5]<<"MBytes\n\n";
	com<<"received start command, now start program\n";
	spiFlashSizeM = testTimes[5];
	PowerOn();

//	flashWp.SetLevel(1);
//	flashWp.SetLevel(1);
//	com<<"factory device ID:"<<spiFlash.DeviceID()<<"\n";
	u16 id = spiFlash.ManufactDeviceID();
	com<<"factory ManufactDevice ID:"<<id<<"\n";
//	com<<"factory JedecDevice ID:"<<spiFlash.JedecDeviceID()<<"\n\n";
//	com<<"factory Unique ID:"<<spiFlash.UniqueID()<<"\n";
	if(id ==0 || id==255 || id==65535 ||id<0x0100)
	{
		com<<"can not find flash!\n";
		while(1);
	}
	u16 allTestTimes = 1;
	double allCostTime = 0;
	float onceTestTime = 0;
	while(1)
	{
		float timeStart = TaskManager::Time();
		onceTestTime = TaskManager::Time();
		if(allTestTimes>testTimes[4])
		{
			com.Setprecision(2)<<"\n\n\n\n\n\nAll Test Complete! time last:"<<allCostTime<<"s,("<<allCostTime/60<<"min,"<<allCostTime/60/60<<"hour)\n\n\n\n\n";
			while(1);
		}
		com<<"\n\n\n\n\nStart test "<<allTestTimes++<<":\n\n\n";
		if(testTimes[3])
		{
			com<<"Test Write Read All, start time:"<<timeStart<<"\n";
			TestWriteReadAll();
		}
		com.Setprecision(2)<<"Test Write Read All Complete, time last:"<<(TaskManager::Time() - timeStart)<<"s\n\n\n\n\n Test Randow Write Read : start time:"<<TaskManager::Time()<<"\n";
		timeStart = TaskManager::Time();
		TestRandomWriteRead1kTimes();
		com.Setprecision(2)<<"\n Test Randow Write Read Complete : time last:"<<(TaskManager::Time() - timeStart)<<"s\n\n\n\n\n Test Powercut: start time:"<<TaskManager::Time()<<"\n";
		timeStart = TaskManager::Time();
		TestPowerCut1kTimesEvery1Hz10Hz100Hz();
		com.Setprecision(2)<<"\n Test Powercut Complete : time last:"<<(TaskManager::Time() - timeStart)<<"s\n\n\n\n\n Test Erase Write "<<(int)testTimes[2]<<" times: start time:"<<TaskManager::Time()<<"\n";
		timeStart = TaskManager::Time();
		TestFirstSectorEraseWrite100kTimes();
		com.Setprecision(2)<<"\n Test First Sector Erase and Write 100k times Complete : time last:"<<(TaskManager::Time() - timeStart)<<"s\n";
		com<<"\n\n\n\n\nTest "<<allTestTimes-1<<" Complete, time last: "<<TaskManager::Time()-onceTestTime<<"s\n";
		allCostTime +=(TaskManager::Time()-onceTestTime);
	}
}

void TestWriteReadAll(void)
{
	u32 errorCount = 0;
	u16 print=0;
	PowerOn();
	com<<"erase all...\n";
	spiFlash.EraseAll();
	com<<"erase complete\n";
	com<<"write data to full chip\n";
	for(u16 i=0;i<256;++i)
		dataToWrite[i]=i;
	u32 chipPagesNum = spiFlashSizeM*1024*1024/256;
	com<<"now start\n";
	for(u32 i=0;i<chipPagesNum;++i)
	{
		spiFlash.WritePage(i*256,dataToWrite,256);
		if(print++>600)
		{
			print = 0;
			com.Setprecision(1)<<i*1.0/chipPagesNum*100<<"% ";
		}
	}
	print = 0;
	com<<"\nwrite full chip complete\n";
	com<<"read full chip:\n";
	for(u32 i=0;i<chipPagesNum;++i)
	{
		spiFlash.Read(i*256,dataRead,256);
		if(print++>600)
		{
			print = 0;
			com.Setprecision(1)<<i*1.0/chipPagesNum*100<<"% ";
		}
		if(memcmp(dataToWrite,dataRead,256)!=0)//error occurred
		{
			++errorCount;
			for(u16 j=0;j<256;++j)
			{
				if(dataToWrite[j]!=dataRead[j])
					com<<"\ndiff address:"<<(int)(i*256+j)<<",should be:"<<dataToWrite[j]<<",actual:"<<dataRead[j]<<"\n";
				TaskManager::DelayMs(10);
			}
		}
	}
	com.Setprecision(0)<<"\nread complete,error num:"<<(double)errorCount<<"\n";
	PowerOff();
}
void TestRandomWriteRead1kTimes(void)
{
	u32 errorCount = 0;
	u16 print=0;
	PowerOn();
	com.Setprecision(0);
	dataToWrite[0]=0xe5;
	com<<"Start test random wirite read\n";
	for(u16 i=0;i<testTimes[0];++i)
	{
		u32 addr = rand()%(spiFlashSizeM*1024*1024);
		spiFlash.EraseSector(addr/4096);
		spiFlash.WritePage(addr,dataToWrite,1);
		spiFlash.Read(addr,dataRead,1);
		if(print++>(int)testTimes[0]/20)
		{
			print = 0;
			com.Setprecision(1)<<i*1.0/(int)testTimes[0]*100<<"% ";
		}
		if(dataRead[0] != dataToWrite[0])
		{
			++errorCount;
			com<<"\nwrite read error,address:"<<(double)addr<<",read:"<<dataRead[0]<<",write:"<<dataToWrite[0]<<",sector:"<<int(addr/4096)<<",position:"<<(int)addr%4096<<"\n";
			TaskManager::DelayMs(10);
//			for(int j=0;j<256;++j)
//				com<<dataRead[j]<<" ";
//			com<<"\n";
		}
	}
	com.Setprecision(0)<<"\nrandom read write test complete,error count:"<<(double)errorCount<<"\n";
	PowerOff();
}
void TestFirstSectorEraseWrite100kTimes(void)
{
	u16 print = 0;
	com<<"Start first sector test!\n";
	PowerOn();
	for(u32 i=0;i<testTimes[2];++i)
	{
		spiFlash.EraseSector(0);
		if(print++>(int)testTimes[2]/500)
		{
			print = 0;
			com.Setprecision(1)<<i*1.0/(int)testTimes[2]*100<<"% ";
		}
		for(int j=0;j<16;++j)
		{
			spiFlash.WritePage(j*256,dataToWrite,256);
		}
		ledRed.Toggle();
	}
	com<<"\n";
	for(int j=0;j<16;++j)
	{
		spiFlash.Read(j*256,dataRead,256);
		com<<".";
		if(memcmp(dataRead,dataToWrite,256)!=0)
		{
			com<<"error ocuured!page:"<<j<<"\n";
		}
	}
	com<<"\ntest write first sector complete!\n";
	PowerOff();
}
void TestPowerCut1kTimesEvery1Hz10Hz100Hz(void)
{
	u8 errorCount=0;
	u16 interval[]={1000,100,10};
	com<<"start power switch test\n";
	for(u8 j=0;j<3;++j)
	{
		u16 print = 0;
		com<<"Start "<<(int)1000/interval[j]<<"Hz test\n";
		PowerOn();
		spiFlash.EraseSector(0);
		for(u16 i=0;i<256;++i)
			dataToWrite[i]=i;
		spiFlash.WritePage(0,dataToWrite,256);

		for(int i=0;i<testTimes[1];++i)
		{
			if(IsPowerOn())
				PowerOff();
			else
				PowerOn();
			TaskManager::DelayMs(interval[j]);
			if(print++>(int)testTimes[1]/interval[j]*3)
			{
				print = 0;
				com.Setprecision(1)<<i*1.0/(int)testTimes[1]*100<<"% ";
			}
			ledRed.Toggle();
		}
		spiFlash.Read(0,dataRead,256);
		
		if(!(memcmp(dataToWrite,dataRead,256)==0))
		{
			++errorCount;
			com<<"\ntest switch with "<<1000/interval[j]<<"HZ failed!\n";
		}
		else
			com<<"\ntest switch with "<<1000/interval[j]<<"HZ success!\n";
	}	
	com<<"power switch test complete! error count:"<<errorCount<<"\n";
}
bool CanWriteRead(void)
{
		PowerOn();
		spiFlash.EraseSector(0);
		for(u16 i=0;i<256;++i)
			dataToWrite[i]=i;
		spiFlash.WritePage(0,dataToWrite,256);
		spiFlash.Read(0,dataRead,256);
		return memcmp(dataToWrite,dataRead,256)==0;
}

void PowerOff(void)
{
	flashPower.SetLevel(0);
	flashPowerGnd.ChangeMode(GPIO_Mode_IN_FLOATING);
}
void PowerOn(void)
{
	flashPower.SetLevel(1);
	flashPowerGnd.ChangeMode(GPIO_Mode_Out_PP);
}
bool IsPowerOn(void)
{
	return flashPower.GetLevel();
}
