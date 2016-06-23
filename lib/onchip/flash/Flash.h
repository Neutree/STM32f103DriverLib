#ifndef  __FLASH_H
#define  __FLASH_H
/**@file Memory.h
  *@brief stm32 flash的读写操作
  *    使用：
  *        * 引入文件到工程中，Memory.cpp Memory.h
  *        * 建立一个Memory对象（注意构造函数，默认是从63K Byte处开始存数，使用半字储存）
  *        * 使用Read和Write函数进行读写，具体参数和返回值见函数说明  
  * 
  *@author neucrack CQUT IOT lib
  *
  */

#include "stm32f10x.h"
#include "stm32f10x_flash.h"


/* Define the STM32F10Xxx Flash page size depending on the used STM32 device */
#if defined (STM32F10X_LD) || defined (STM32F10X_MD)
  #define MEMORY_PAGE_SIZE  (uint16_t)0x400  /* Page size = 1KByte */
#elif defined (STM32F10X_HD) || defined (STM32F10X_CL)
  #define MEMORY_PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */
#endif


/* Page status definitions */
#define MEMORY_STATUS_ERASED                  ((uint16_t)0xFFFF)     /* PAGE is empty */
#define MEMORY_STATUS_RECEIVE_DATA            ((uint16_t)0xEEEE)     /* PAGE is marked to receive data */
#define MEMORY_STATUS_VALID_PAGE              ((uint16_t)0x0000)     /* PAGE containing valid data */





class flash
{
private:
	
	bool mUseHalfWord;//储存字长  16/32 默认16位
	uint32_t mStartAddress;//开始储存的地址
public:
///////////////////
///构造函数
///@param startAddress 存储数据的开始位置 默认：0x08010000 即63k byte 处
///@param useHalfWord 单个数据储存的长度 true:16位 false:32位  默认：16位
///////////////////
flash(uint32_t startAddress=(0x08000000+63*MEMORY_PAGE_SIZE),bool useHalfWord=true);


///////////////////////
///读取储存器中特定位置的值
///@param -relativeAddress 相对于开始地址的地址
///@param -Data 读出的数据存放的地址
///@retval -1 : 读取成功 -0：读取失败
///////////////////////
bool Read(uint16_t relativeAddress, uint8_t* Data,u16 length);
		
//position为一页中的位置，只能是偶数或者0
bool Read(u16 pageNumber,u16 position,u16* data,u16 length);


bool Read(uint16_t relativeAddress, uint32_t* Data,u16 length);
		
///////////////////////
///向储存器中特定位置写值
///@param -pageNumber 相对于开始地址的页地址
///@param -Data 将要写入的数据
///@attention 如果构造构造函数的参数useHalfWord为true时，会现将其转换为u16再储存，否则会转换成u32再储存
///@retval -1 : 写入成功 -0：写入失败
///////////////////////
bool Write(uint16_t pageNumber, uint8_t* Data,u16 length);
		
//position为一页中的位置，只能是偶数或者0		
bool Write(uint16_t pageNumber,u16 position,uint16_t* data,u16 length);
		

bool Write(uint16_t pageNumber, uint32_t* Data,u16 length);
		

//字符串的存储于读取,参数：页码，页码中位置，字符串
bool Write(uint16_t pageNumber,u16 position,char* str);
bool Read(uint16_t pageNumber,u16 position,char *str);
bool Clear(uint16_t pageNumber);

};


#endif

