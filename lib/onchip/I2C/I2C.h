/**
  *@file I2C.h
  *@author Neucrack
  *@breif I2C driver for stm32f10x
  *   how to use it:
  *      * import 'I2C.cpp', 'Interrupt.cpp' to your project and add 'I2C.h', 'Interrupt.h', 'FIFOBuffer.h' to your project lib,
  *      * creat a I2C object such as 
  *                                  ```C++ 
  *                                       I2C iic(1,false);
  *                                  ```
  *      * use the object to operate iic devices @see I2C. such as:
  *                                  ```C++
  *                                      iic.iic.AddCommand(0xd0,0x75,0,0,temp,1);
  *                                      if(!iic.StartCMDQueue())  //or if(!iic.StartCMDQueue(3)) (add time out parameter)
  *                                      {
  *                                         iic.Init();
  *                                      }	
  *                                  ```
  *@copyright CQUT IOT Lib all rights reserved
  */



#ifndef __I2C_H
#define __I2C_H

//include files
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_dma.h"
#include "FIFOBuffer.h"
#include "Delay.h"
#include "TaskManager.h"
#include "Sensor.h"

	/** @defgroup I2C_Configuration_Block
	  * @{
	  */
/****************************************************************************************/

						#define I2C_QUEUE_MAX_SIZE    40 //The max size of i2c command queue
						#define I2C_USE_DMA              //uncomment it if you want to use DMA for USART
						#define MAX_INTERVAL_NUMBER   5  //相同命令执行的间隔时间的最大的记录数
//						#define USE_ERROR_CALLBACK       //  use iic error  callback ** this one can not be used now **
							
//						#define DEBUG
/****************************************************************************************/

	/**
	  *@}
	  */
#ifdef DEBUG
	#include "USART.h"
	extern USART usart1;
	
#endif
/////////////////////////
///IIC state enum
////////////////////////
typedef enum
{
	STATE_DISABLED = 0x00,  //peripheral is disabled
    
	STATE_READY    = 0x01, //iic bus is free and ready to communicate

	STATE_BUSY     = 0x02, //iic bus is busy	    

	STATE_ERROR    = 0x10, //iic bus occured some error signal

	STATE_SEND_ADW = 0x20, //need to send slave address and write signal
	
	STATE_SEND_ADR = 0x21, //need to send slave address and read signal
	
	STATE_SEND_DATA= 0x22, //need to send data that need to be send to slave(slave register address  or command data)
	
	STATE_REVEIVE_DATA= 0x30 //receiving data
	
}I2C_State;

/////////////////////
///iic command type enum
/////////////////////
typedef enum
{
	I2C_READ_BYTE   = 0x01,	//Read one byte from the specified register of slave 
	I2C_READ_BYTES  = 0x02,	//Read some bytes from the specified register of slave
	I2C_WRITE_BYTE  = 0x04,	//write one byte to the specified register of slave
	I2C_WRITE_BYTES	= 0x05,	//write some bytes to the specified register of slave
	I2C_WRITE_CMD   = 0x06,  //write command(one byte or more) to the slave(no register address specified)
}IIC_CMD_Type;


/////////////////////
///iic command struct
////////////////////
typedef struct 
{ 
	IIC_CMD_Type cmdType;			//iic command tye
	u8 slaveAddr;		            //slave adress
	u8 DataOut[5];	                //the data will be send out to slave (onlu for few bytes(here the max length is 5))
	u8 outDataLen;                  //the length of out data
	u8* pDataIn;                    //the save address of received data
	u8 inDataLen;                   //the length of received data
	bool isRecordInterval;          //whether record time,是否记录两次执行该命令的间隔时间，为0代表不记录，大于零代表记录，数值为记录的数组的下标
	Sensor* pDevice;                //传感器设备指针
}I2C_Command_Struct;



/////////////////////////////
///I2C Driver class
/////////////////////////////
class I2C
{
	private:
		I2C_TypeDef* mI2C;        //i2c device name
		u32          mSpeed;      //i2c bus speed
		bool         mRemap;      //whether gpio pin remap
		bool         mUseDma;     //whether use DMA
		uint8_t      mPriority[6];//priority table
		I2C_State    mState;      //i2c bus state flag
		u16 mIndex_Send;          //send data flag
		FIFOBuffer<I2C_Command_Struct,I2C_QUEUE_MAX_SIZE> mCmdQueue;       //iic cmd queue
		I2C_Command_Struct mCurrentCmd;                       //current command should be execute variable
	#ifdef I2C_USE_DMA
		uint32_t mDmaRxTcFlag,mDmaTxTcFlag;                   //DMA tranmite complete flag adress
		DMA_Channel_TypeDef *mDmaTxChannel,*mDmaRxChannel;    //iic dma channel 
		DMA_InitTypeDef mDMA_InitStructure;                   //i2c dma configuration 
	#endif
	
	
		/////////////////////////
		///soft reset iic Peripheral(reset register related to iic peripheral)
		/////////////////////////
		void Soft_Reset(void);
		void I2CGPIODeInit(uint16_t sclPin,uint16_t sdaPin);
		void I2CGPIOInit(uint16_t sclPin,uint16_t sdaPin);
		///////////////////////
		///check the iic bus if was locked by slave,if do,clear it
		///@param i2cClock iic clock
		///@param sclPin iic scl pin
		///@param sdaPin iic sda pin
		///@retval true: if check and fix succeed false: error occur, check and fix fail
		///////////////////////
		bool I2C_CHACK_BUSY_FIX(uint32_t i2cClock,uint16_t sclPin,uint16_t sdaPin);
		void DelayUs(u32 nus);
		void DelayMs(u16 nms);
		void DelayS(u32 ns);
	public:
		
		/////////////////////////////////////
		///@breif IIC initialization function
		///@param i2cNumber select which IIC peripheral to init
		///@param useDMA true: if defined I2C_USE_DMA @see I2C_Configuration_Block use DMA to operate iic device false: do not use dma for iic 
		///@param speed define the speed of iic bus 
		///@param remap select whether to use iic GPIO remap ,only of I2C1,I2C2 do not have remap GPIO @see I2C_Pin_Table at the end of this file
		///@param priorityGrouop Choose which priorityGroup to use,payattention,only one should be used in one project
		///@param preemprionPriorityEvent Iic event interrupt preemprion priority should bigger than iic error and iic dma priority
		///@param subPriorityEvent IIC event interrupt subpriority
		///@param preemprionPriorityError IIC error interrupt preemprion priority should smaller than iic error and iic dma priority
		///@param subPriorityError IIC error interrupt subpriority
		///@param preemprionPriorityDma IIC dma interrupt preemprion priority should between  iic error and iic dma priority
		///@param subPriorityDma IIC dma interrupt subpriority
		///@retval Is initialization success
		///@attention only one group should be used in one project. 
		///           The bigger the priority value,the lower level priority.Every group's detail @see NVIC_Priority_Table at the end of this file
		/////////////////////////////////////
		I2C(u8 i2cNumber=1,bool useDMA=false,u32 speed=400000,u8 remap=0,u8 priorityGroup=3,
					uint8_t preemprionPriorityEvent=2,uint8_t subPriorityEvent=0,
					uint8_t preemprionPriorityError=0,uint8_t subPriorityError=0,
					uint8_t preemprionPriorityDma=1,uint8_t subPriorityDma=0);
	
		/////////////////////////////////////
		///@breif IIC initialization function
		///@retval Is initialization success
		/////////////////////////////////////
		bool Init();
		
		
		///////////////////////////
		///@breif Get the iic health details 
		///@retval 1:health 0：something wrong
		///////////////////////////
		u8 IsHealth();
		
		///////////////////////////
		///@breif Get the status of queue
		///@retval 0:not ok sending  1:if the queue is empty and the queue status is ready  2:error occured
		///////////////////////////
		u8 IsSendOk();
		
		//////////////////////////
		///wait until transmission complete
		///@param errorReset if init the iic bus when error occured
		///@param errorClearCmdQueue If clear the iic command queue when error occured
		///@param errorRestart Whether restart the queue until the queue is empty if occured any error
		///      * if this param is set to true, no matter param 'errorReset' if set to true, 
		///     it will reset and initialize iic first, then execute the remaining command in the queue 
		///      * if this param is set to false, when this function return false, the queue may not empty because of error
		///@retval true:transmission complete  false: occured some wrong
		//////////////////////////
		bool WaitTransmitComplete(bool errorReset=true,bool errorClearCmdQueue=false,bool errorResart=true);
		
		
		/////////////////////////////
		///@param slaveAddr The address of slave device
		///@param registerAddr The register adress of slave,if you don't write register,just fill it with 0xff
		///@param dataWrite The head adress of send data array
		///@param sendNum The length of send data
		///@param dataRead The head  adress of received data array
		///@param receiveNum The length of receive data
		///@param isRecordInterval 是否记录两次执行该命令的间隔时间，0为不记录，大于零则为记录，值为储存间隔值数组的下标，使用Interval()获得两次执行相同命令的间隔时间
		/////////////////////////////
		bool AddCommand(u8 slaveAddr,u8 registerAddr, u8* dataWrite, u8 sendNum,u8* dataRead, u8 receiveNum,bool isRecordInterval=false,Sensor* pDevice = 0);
		
		////////////////////////////////////////////////////
		///在调用开始执行命令队列中的命令时会对当前的总线状态进行检查，
		///若连续timeOutMaxTime次都是一样的状态（STATE_READY除外），则判定为超时 
		///befor start signal,it will check the status of iic bus 
		///if the status timeOutMaxTime consecutive times the same,it will be regarded as an time out error has occurred 
/*		///@param timeOutMaxTime 超时计时最大值，可以不使用，为防止出现有时出现无法检测到的错误的情况发生，建议使用。
		///       若连续timeOutMaxTime次调用此函数时iic的状态都是同一状态，则判定为超时（引起的原因可能是总线出现错误，但是不会产生错误中断,比如从机vcc断开）
		///       if the status timeOutMaxTime consecutive times the same,it will be regarded as an time out error has occurred 
*/		///@retval If the success to send start signal 1:signal will be send 0:some error occured 2: queue is empty
		///@attention The value of timeOutMaxTime should be valued Correctly, set the value according to the Interval of invoke this function
		///           eg: if Interval is too short and timeOutMaxTime is too little,it can due to iic bus's error
		///////////////////////////////////////////////////
		u8 StartCMDQueue(/*uint32_t timeOutMaxTime=80*/);
		
		
		///////////////////////////
		///@breif clear all i2c command in the queue
		///////////////////////////
		void ClearCommand(void);  
		
		
		/////////////////////////
		///i2c event interrupt handler
		///@attention It should be put in the iic event interrupt function
		////////////////////////
		void EventIRQ(void);     
		
		/////////////////////////
		///i2c error interrupt handler
		///@attention It should be put in the iic error interrupt function
		////////////////////////
		void ErrorIRQ(void);  
		
	#ifdef I2C_USE_DMA
		/////////////////////////
		///i2c send DMA interrupt function
		///@attention It should be put in the iic send DMA interrupt function
		////////////////////////
		void DmaTxIRQ(void);
		
		/////////////////////////
		///i2c reveive DMA interrupt function
		///@attention It should be put in the iic reveive DMA interrupt function
		////////////////////////
		void DmaRxIRQ(void);
	#endif
		////////////////////////////
		///return the number of iic
		///@retval return the number of iic
		/////////////////////////////
		u8 GetI2CNumber();
};


/* I2C Errors TypeDef */
typedef enum
{
  I2C_ERR_NONE      = 0x0000, /*!<No Error: This is the default state for an Idle peripheral */

  I2C_ERR_TIMEOUT   = 0x00FF, /*!<Timeout error: The specified timeout has been elapsed without 
                                         any response (expected flag or data didn't happen at expected time). */

  I2C_ERR_BERR      = 0x0100, /*!<Bus error: This error occurs when I2C peripheral detects an external
                                       Stop or Start condition during address or data transfer. In this case:
                                          - The BERR bit is set and an interrupt is generated if the ITERREN bit is set.
                                       In Slave mode: 
                                         data are discarded and the lines are released by hardware:
                                          - In case of a misplaced Start, the slave considers it is a restart and 
                                            waits for an address, or a Stop condition.
                                          - In case of a misplaced Stop, the slave behaves like for a Stop condition and 
                                           the lines are released by hardware.
                                       In Master mode: 
                                         the lines are not released and the state of the current transmission is not 
                                         affected. It is up to the software to abort or not the current transmission.
                                       
                                       Software Clearing sequence for the BERR bit:      
                                         1. Writing '0' to this bit  */
                                            
                                                      
  I2C_ERR_ARLO        = 0x0200, /*!<Arbitration Lost error: This error occurs when the I2C interface detects 
                                         an arbitration lost condition. In this case:
                                          - The ARLO bit is set by hardware (and an interrupt is generated if the 
                                            ITERREN bit is set).
                                         the I2C Interface goes automatically back to slave mode (the M/SL bit 
                                         is cleared). 
                                         When the I2C loses the arbitration, it is not able to acknowledge its slave
                                         address in the same transfer, but it can acknowledge it after a repeated 
                                         Start from the winning master.
                                         Lines are released by hardware.
                                              
                                         Software Clearing sequence for the BERR bit:      
                                          1. Writing '0' to this bit  */
                                                  
  I2C_ERR_AF          = 0x0400, /*!<Acknowledge Failure : This error occurs when the interface detects 
                                         a non-acknowledge bit. In this case:
                                          - The AF bit is set and an interrupt is generated if the ITERREN bit 
                                            is set.
                                         A transmitter which receives a NACK must reset the communication:
                                          - If Slave: lines are released by hardware.
                                          - If Master: a Stop or repeated Start condition must be generated 
                                            by software.
                                                 
                                         Software Clearing sequence for the ARLO bit:
                                         1. Writing '0' to this bit */                                        
                                                      
  I2C_ERR_OVR          = 0x0800, /*!<Overrun/Underrun error: An overrun error can occur in slave mode when clock 
                                          stretching is disabled and the I2C interface is receiving data. The interface has
                                          received a byte (RxNE=1) and the data in DR has not been read, before the next 
                                          byte is received by the interface. 
                                          In this case:
                                          The last received byte is lost.
                                           - In case of Overrun error, software should clear the RxNE bit and the transmitter 
                                             should retransmit the last received byte.
                                          
                                          Underrun error can occur in slave mode when clock stretching is disabled and the 
                                          I2C interface is transmitting data. The interface has not updated the DR with the 
                                          next byte(TxE=1), before the clock comes for the next byte. In this case:
                                           - The same byte in the DR register will be sent again.
                                           - The user should make sure that data received on the receiver side during an 
                                             underrun error are discarded and that the next bytes are written within the 
                                             clock low time specified in the I2C bus standard.
                                          For the first byte to be transmitted, the DR must be written after ADDR is 
                                          cleared and before the first SCL rising edge. If not possible, the receiver 
                                          must discard the first data.
                                      
                                       Software Clearing sequence for the ARLO bit:
                                        1. Writing '0' to this bit */
                                                  
 }I2CErrorTypeDef;


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

	/** @defgroup I2C_Pin_Table 
	  * @{
	  */

	/**
	@code  
	 The table below gives the allowed values of the pre-emption priority and subpriority according
	 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
	  =========================================
		|  I2C | remap=false |  remap=true |
		|------|-------------|-------------|
		| I2C1 |SCL  PB6     |   SCL  PB8  |
		|      |SDA  PB7     |   SDA  PB9  |
		|======|=============|============ |
		| I2C2 |SCL  PB10    |   --------  |
		|      |SDA  PB11    |   --------  |
	  =========================================
	@endcode
	*/

	/**
	  * @}
	  */

#endif

