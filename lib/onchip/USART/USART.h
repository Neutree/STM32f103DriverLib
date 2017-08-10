
/**
*@file USART.h
*@author 2015-20-28 Neutree
*@version v1.1
*@brief stm32f10x串口驱动文件，使用时引入 USART.h USART.cpp FIFOBuffer.h Interrupt.h Interrupt.cpp 五个文件，
*       然后根据需要配置USART.h中开头的配置部分（缓冲区大小配置）
*       中断均由Interrupt.cpp中管理，若需要自定义，请自行编辑中断函数
*       然后定义对象初始化，既可以使用
*@attention MD容量的片子只能使用USART1、2、3，HD可以使用4，如果MD使用了4，程序会进入Interrupt.CPP中的assert_failed()函数中
*@copyright CQUTIOTLIB all right reserved
*
*/
#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "FIFOBuffer.h"
#include "GPIO.h"
#include "string.h"



/**
  *@addtogroup USART_CONFIGURATION
  *@{
  */
/******************************************************************************************************/
							/****configuration，使用前请自行配置****/
		#define USART_TX_BUFFER_SIZE     256              //USART BUFFER FIFO SIZE
		#define USART_RX_BUFFER_SIZE     256              //USART BUFFER FIFO SIZE
		#define USART_DMA_TX_BUFFER_SIZE 20             //USART DMA BUFFER SIZE
		
/*******************************************************************************************************/

/**
  *@}
  */

/**
* @author jason
* @brief 串口中断类型
*/
typedef enum
{
	USART_RX_IRQ,				//串口接收中断
	USART_TX_IRQ,				//串口发送中断
	USART_IDLE_IRQ				//串口空闲中断
}UsartIrqType;


  
 /**
   *@brief A USART driver class for STM32F103 series
   *
   */
class USART
{
private:
	FIFOBuffer<u8,USART_TX_BUFFER_SIZE>  bufferTx;
	FIFOBuffer<u8,USART_RX_BUFFER_SIZE>  bufferRx;
	u8 bufferTxDma[USART_DMA_TX_BUFFER_SIZE];
	DMA_Channel_TypeDef* dmaChannelTx;				//dma通道
	USART_TypeDef* usart;							//串口号
	uint32_t dmaTcFlagChannel;						//dma传输完成中断标志位
	uint32_t dmaGlFlagChannel;						//dma中断标志位
	bool isBusySend;
	bool mUseDma;
	unsigned char mPrecision;

	//jason add
	uint32_t mBaud;
	GPIO mTx;
	GPIO mRx;

public:

	///////////////////////////
	///@brief Destructor
	///////////////////////////
	~USART();
	
	
	////////////////////
	///@brief 带参数的构造函数
	///@param USART	 选择串口编号 1-3 ,对应的引脚请看下面重映射处        default:1
	///@param baud -串口的波特率              default:9600
	///@param useDMA  true:if you want to send data by DMA false:don't use dma to send data
	///@param remap -if remap 0x00:no remap  0x01:half remap 0x11:remap  default:0x00 . which pin @see USART_Pin_Table at the bottom of this file
	///@param Prioritygroup -中断优先级分组   default:3  优先级分组详情请看文件末尾
	///@param preemprionPriority -抢占优先级  default:7  优先级分组详情请看文件末尾
	///@param subPriority -响应优先级         default:1  优先级分组详情请看文件末尾
	///@param dmaPriority set the priority of DMA default:3(low) (0:DMA_Priority_VeryHigh 1:DMA_Priority_High 2:DMA_Priority_Medium 3:DMA_Priority_Low)
	///@param  parity usart parity value: USART_Parity_No  USART_Parity_Even  USART_Parity_Odd      default:USART_Parity_No
	///@param  wordLength   USART_WordLength_9b  USART_WordLength_8b                                default:USART_WordLength_8b
	///@param  stopBits  USART_StopBits_1 USART_StopBits_0_5  USART_StopBits_2  USART_StopBits_1_5  default:USART_StopBits_1
	////////////////////
	USART(u8 USART=1,uint32_t baud=9600,bool useDMA=false, u8 remap=0x00,u8 Prioritygroup=3,uint8_t preemprionPriority=7,uint8_t subPriority=1,u8 dmaPriority=3
			,uint16_t parity=USART_Parity_No,uint16_t wordLength=USART_WordLength_8b, uint16_t stopBits=USART_StopBits_1);

	///////////////////
	///@brief 串口发送数据
	///@param pbuffer 需要发送数据的首地址
	///@param size 需要发送的数据的长度
	///@retval 发送是否成功 -false:发送失败（队列溢出）  -true:发送成功（即将发送）
	///////////////////
	bool SendData(uint8_t *pbuffer, uint32_t size);

	USART& PrintHex(uint8_t *pbuffer, uint32_t size,const char* str);
	USART& PrintHex(uint8_t pbuffer);


	///////////////////
	///@brief 获取缓冲区中的数据
	///@param buffer -数据要保存到的地方
	///@param number -数据个数
	///@retval 获取数据是否成功
	///////////////////
	bool GetReceivedData(u8* buffer,u16 number);

	///@brief 获取所用的USART
	///@retval 正在使用的USART
	USART_TypeDef* GetUSART();

	//////////////////////////
	///@bief 设置波特率
	///@param baudRate 波特率大小
	//////////////////////////
	void SetBaudRate(uint32_t baudRate);
	
	//////////////////////////
	///@bief 得到波特率
	///@retval 波特率大小
	//////////////////////////
	uint32_t GetBaudRate();
	
	/////////////////////
	///@brief 获取接收区缓冲区有效数据的长度
	///@retval -缓冲区有效数据的长度
	/////////////////////
	u16 ReceiveBufferSize();
	
	
	/////////////////////
	///@brief 获取发送缓冲区有效数据的长度
	///@retval -缓冲区有效数据的长度
	/////////////////////
	u16 SendBufferSize();
	
	
	//////////////////////////
	///brief 清空接收缓冲区中的数据
	//////////////////////////
	void ClearReceiveBuffer();
	
	//////////////////////////
	///brief 清空发送缓冲区中的数据
	//////////////////////////
	void ClearSendBuffer();
	
	//////////////////////
	///@brief 串口中断处理函数
	//////////////////////
	UsartIrqType Irq();
	
	
	/////////////////////
	///@brief dma中断处理函数
	/////////////////////
	void DmaIrq();
	
	
	///////////////////////////
	///@brief print integer reload
	///@param val the integer value that will be print to USART as characters
	///////////////////////////
	USART& operator<<(int val);
	
	
	/////////////////////////////
	///@brief print double reload
	/////////////////////////////
	USART& operator<<(double val);
	
	
	////////////////////////////
	///set the precision of double output
	////////////////////////////
	USART& Setprecision(const unsigned char precision);
	
	///////////////////////////
	///@brief print string reload
	///@param the string's first character adress value that will be print to USART as characters
	///////////////////////////
	USART& operator<<(const char* pStr);
	
	///////////////////////////
	///@brief print character reload
	///@param the string's first character adress value that will be print to USART as characters
	///////////////////////////
	USART& operator<<(const char charactor);

	////////////////////////////////
	///@brief Use DMA or not
	///@retval true:use dma  false:don't use dma 
	////////////////////////////////
	bool UseDma();

	////////////////////////////////
	///@brief Get the Tx that used by USART
	///@retval the reference of Tx GPIO 
	////////////////////////////////
	GPIO& GetmTx();
	
	////////////////////////////////
	///@brief Get the Rx that used by USART
	///@retval the reference of Rx GPIO  
	////////////////////////////////
	GPIO& GetmRx();
	
};

	/** @defgroup NVIC_Priority_Table 
	  * @{
	  */

	/**
	@code  
	 The table below gives the allowed values of the pre-emption priority and subpriority according
	 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
	  ============================================================================================================================
		NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
	  ============================================================================================================================
	   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
							 |                                   |                             |   4 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------
	   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
							 |                                   |                             |   3 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------    
	   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
							 |                                   |                             |   2 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------    
	   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
							 |                                   |                             |   1 bits for subpriority
	  ----------------------------------------------------------------------------------------------------------------------------    
	   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
							 |                                   |                             |   0 bits for subpriority                       
	  ============================================================================================================================
	@endcode
	*/

	/**
	  * @}
	  */

	/** @defgroup USART_Pin_Table 
	  * @{
	  */
	  
		  /**
				===========================================================================
									|			remap value				|
						usartName	|	0x00	|	0x01	|	0x11	|
						usart1Tx	|	PA9		|	PB6		|			|
						usart1Rx	|	PA10	|	PB7		|			|
						usart2Tx	|	PA2		|	PD5		|			|
						usart2Rx	|	PA3		|	PD6		|			|
						usart3Tx	|	PB10	|	PC10	|	PD8		|
						usart3Rx	|	PB11	|	PC11	|	PD9		|
				===========================================================================
			*/
		
	/**
	  * @}
	  */

#endif
