#include "I2C.h"
#include "Interrupt.h"
#ifdef DEBUG
	#include "I2C.h"
#endif
///////////////////////////
///获取I2C设备的通信健康状况
///@retval 1:健康 0：出现错误
///////////////////////////
u8 I2C::IsHealth()
{
	if(mState==STATE_ERROR)
		return 0;
	else
		return 1;
}
///////////////////////////
///@breif Get the status of queue
///@retval 0:not ok sending  1:if the queue is empty and the queue status is ready  2:error occured
///////////////////////////
u8 I2C::IsSendOk()
{	
	if(!IsHealth())
		return 2;
	if(mCmdQueue.Size()>0)
		return 0;
	return 1;
}

//////////////////////////
///wait until transfer complete
//////////////////////////
bool I2C::WaitTransmitComplete(bool errorReset,bool errorClearCmdQueue,bool errorResart)
{
	u16 timeOut=0;
	u8 status=1;
	u8 temp;
	while(1)
	{
	waitLoop:
		temp=this->IsSendOk();
		
		if(temp==1)
		{
			status=1;
			if(this->mState==STATE_READY)//等待，直到命令执行完毕
				break;
			else if(this->mState==STATE_ERROR)//如果在最后一个命令执行过程中出现错误
			{
				status=0;
				break;
			}
		}
		else if(temp==2)
		{
			status=0;
			break;
		}
		++timeOut;
		if(timeOut>65534)
		{			
			status=0;
			break;
		}
	}
	if(status!=1)
	{
		if(errorClearCmdQueue)
			this->ClearCommand();//清空当前队列里的命令
		if(errorResart)
		{
			errorReset=true;
		}
		if(errorReset)
		{
			this->Init();
		}
		if(errorResart)
		{
			if(!this->IsSendOk())//还有数据，发送开始信号,知道队列里的数据发送完毕
			{
				this->StartCMDQueue();
			//  WaitTransmitComplete(true,false,true);  递归函数，有造成内存浪费的嫌疑，换成了不常用的goto，为了节约内存
				goto waitLoop;
			}
		}
		return false;
	}
	return true;
}

bool I2C::AddCommand(u8 slaveAddr,u8 register_addr, u8* data_write, u8 sendNum,u8* data_read, u8 receiveNum,bool isRecordInterval,Sensor* pDevice)
{
	I2C_Command_Struct IIC_CMD_Temp;	
	u8 i;	
	if(register_addr==0xff)//没有从机寄存器地址，发送命令模式
	{
		IIC_CMD_Temp.cmdType=I2C_WRITE_CMD;
		IIC_CMD_Temp.inDataLen=0;
		IIC_CMD_Temp.outDataLen=sendNum;
		IIC_CMD_Temp.pDataIn=0;
		for(i=0;i<sendNum;++i)
			IIC_CMD_Temp.DataOut[i]=(*data_write)++;
		IIC_CMD_Temp.slaveAddr=slaveAddr;
	}
	else                //
	{
		if(sendNum>=1)//发送模式
		{
			if(sendNum==1)
				IIC_CMD_Temp.cmdType=I2C_WRITE_BYTE;
			else
				IIC_CMD_Temp.cmdType=I2C_WRITE_BYTES;
			IIC_CMD_Temp.inDataLen=0;
			IIC_CMD_Temp.outDataLen=sendNum+1;
			IIC_CMD_Temp.pDataIn=0;
			IIC_CMD_Temp.DataOut[0]=register_addr;
			for(i=1;i<=sendNum;++i)
			{
				IIC_CMD_Temp.DataOut[i]=(*data_write);
				++data_write;
			}
			IIC_CMD_Temp.slaveAddr=slaveAddr;
		}
		else//接收模式
		{
			if(receiveNum>1)//接收多个字节
			{
				IIC_CMD_Temp.cmdType=I2C_READ_BYTES;
			}
			else if(receiveNum==1)//接收一个字节
			{
				IIC_CMD_Temp.cmdType=I2C_READ_BYTE;
			}
			IIC_CMD_Temp.inDataLen=receiveNum;
			IIC_CMD_Temp.outDataLen=1;
			IIC_CMD_Temp.pDataIn=data_read;
			IIC_CMD_Temp.DataOut[0]=register_addr;
			IIC_CMD_Temp.slaveAddr=slaveAddr;
		}
	}
	IIC_CMD_Temp.isRecordInterval = isRecordInterval;//是否记录两次执行该命令的间隔时间
	IIC_CMD_Temp.pDevice = pDevice;
	mCmdQueue.Put(IIC_CMD_Temp);//将命令加入队列
	return true;
}

u8 I2C::StartCMDQueue(/*uint32_t timeOutMaxTime*/)
{	
	/*static u8 I2C_time_out_count=0;//用来为发送数据时计数，每次进入都保持一个状态，当超过一定数就是出错
	static u8 I2C_Status_Before=0;*/
	
	if(mState==STATE_ERROR)//总线出错了
	{
		return 0;
	}
	else if(mState==STATE_SEND_DATA)
	{
		if((this->mI2C->SR1==0) && (this->mI2C->SR2==0))
		{
			mState=STATE_ERROR;
			this->Soft_Reset();
		}
	}
//	if(mState!=STATE_READY && I2C_Status_Before==mState)//I2C现在的状态和之前的状态相同
//	{
//		++I2C_time_out_count;
//		if(I2C_time_out_count>timeOutMaxTime)
//		{
//			I2C_time_out_count=0;
//			mState=STATE_ERROR;
//			this->Soft_Reset();
//			return 0;
//		}
//	}
//	else                                 //与之前的值不相同，赋新的值
//		I2C_Status_Before=mState;
	
	
	if((mI2C->SR2&0x03) == 0x02)//从模式、busy，错误的情况
	{
		mState=STATE_ERROR;
		this->Soft_Reset();
		return 0;
	}
	if(mState==STATE_READY && mCmdQueue.Size()>0 )//IIC状态为空闲，并且队列不为空，则发送起始信号
	{
		if(I2C_GetFlagStatus(mI2C, I2C_FLAG_BUSY))//寄存器中状态为busy，这是不应该存在的情况，因为mState标记为正常状态，说明出错，应该将状态置为错误，并返回错误状态
		{
			mState = STATE_ERROR;
			this->Soft_Reset();
			return 0;
		}
		mCmdQueue.Get(mCurrentCmd);//下一个命令出队
		mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
		mIndex_Send = 0;//发送、接收计数下标清零
		I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
		
		I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号
		return 1;
	}
	else if(mCmdQueue.Size()==0)//队列为空
		return 2;
	return 1;
}

void I2C::ClearCommand()
{
	mCmdQueue.Clear();
}
void I2C::EventIRQ()
{
	static uint32_t I2C_Status=0;
	static u8 timeOut=0;
	I2C_Status = I2C_GetLastEvent(mI2C);
	switch(I2C_Status)//查询中断事件类型
	{
		case I2C_EVENT_MASTER_MODE_SELECT: //EV5   //SB、BUSY、MSL位置位	
			if(mState==STATE_SEND_ADW)//发送从机地址+写信号
			{	
			#ifdef I2C_USE_DMA
				if(mUseDma)
				{
					mI2C->CR2 &= ~I2C_IT_BUF; //使用DMA,关闭BUF中断
					mI2C->CR2 |= I2C_CR2_DMAEN; //使能IIC DMA传输
				}
			#endif
				I2C_Send7bitAddress(mI2C,mCurrentCmd.slaveAddr,I2C_Direction_Transmitter);
				mState = STATE_SEND_DATA;//状态设置为需要发送数据（发送寄存器地址）
			#ifdef I2C_USE_DMA
				if(mUseDma)
				{
						/* I2Cx Common Channel Configuration */
					if(mCurrentCmd.cmdType>=I2C_WRITE_BYTE)//写模式
					{
						mDMA_InitStructure.DMA_BufferSize = mCurrentCmd.outDataLen;
					}
					else                                        //读模式 设置长度为1 因为发送一个字节后需要再次发送起始信号
					{
						mDMA_InitStructure.DMA_BufferSize = 1;
					}
					mDMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)&mCurrentCmd.DataOut[0];
					mDMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralDST;
					
					DMA_Init(mDmaTxChannel,&mDMA_InitStructure);
					
					DMA_Cmd(mDmaTxChannel, ENABLE);
				}
			#endif
				
			}
			else if(mState==STATE_SEND_ADR)//发送从机地址+读信号
			{
				I2C_Send7bitAddress(mI2C,mCurrentCmd.slaveAddr,I2C_Direction_Receiver);
				mState = STATE_REVEIVE_DATA;//状态设置为需要发送数据（发送寄存器地址）
			#ifdef I2C_USE_DMA
				if(mUseDma)
				{
					if( mCurrentCmd.cmdType==I2C_READ_BYTE)//接收一个字节，不能使用DMA，使用中断的方式
					{
						mI2C->CR2 |= I2C_IT_BUF; //接收一个字节，打开 BUF中断，不使用DMA
						mI2C->CR2 &= ~I2C_CR2_DMAEN; //失能IIC DMA传输
					}
					else //接收的字节数大于1，使用DMA
					{
						mI2C->CR2|=I2C_CR2_LAST;//使能接收最后一位时发送NACK
						mI2C->CR2 &= ~I2C_IT_BUF; //使用DMA,关闭BUF中断
						mI2C->CR2 |= I2C_CR2_DMAEN; //使能IIC DMA传输
						
						mDMA_InitStructure.DMA_BufferSize = mCurrentCmd.inDataLen;
						mDMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)mCurrentCmd.pDataIn;
						mDMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;
						
						DMA_Init(mDmaRxChannel,&mDMA_InitStructure);
						
						DMA_Cmd(mDmaRxChannel, ENABLE);
						
					}
				}
			#endif
			}
			break;
		
		
		case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED://EV6	//已: I2C_Send7bitAddress(W)   应: I2C_SendData()
			#ifndef I2C_USE_DMA
			//			I2C->CR1 |= I2C_CR1_PE;
			I2C_SendData(mI2C,mCurrentCmd.DataOut[0]);//发送寄存器地址或者命令
			++mIndex_Send;//发送计数下标后移
			--mCurrentCmd.outDataLen;//需要发送的数据长度减1
			#else
			if(mUseDma==false)
			{
				//			I2C->CR1 |= I2C_CR1_PE;
				I2C_SendData(mI2C,mCurrentCmd.DataOut[0]);//发送寄存器地址或者命令
				++mIndex_Send;//发送计数下标后移
				--mCurrentCmd.outDataLen;//需要发送的数据长度减1
			}
			#endif
			break;
		
		
		case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED://EV6			//已: I2C_Send7bitAddress(R)		
			if(mCurrentCmd.cmdType==I2C_READ_BYTE)//只接收一个字节
			{
				I2C_AcknowledgeConfig(mI2C,DISABLE);//关应答
				I2C_GenerateSTOP(mI2C,ENABLE);//产生停止信号
			}
			break;
		
		
		case I2C_EVENT_MASTER_BYTE_RECEIVED://EV7   //数据到达，接收数据
		if(mState==STATE_REVEIVE_DATA)//防止标志没来得及清掉，重复进入
		{
		#ifdef I2C_USE_DMA
			if(mUseDma)
			{
				
					if(mCurrentCmd.cmdType==I2C_READ_BYTE)//只接收一个字节
					{
						(*mCurrentCmd.pDataIn) = I2C_ReceiveData(mI2C); //保存接收到的数据
					
						mI2C->CR2 &= ~I2C_IT_BUF; //使用DMA,关闭BUF中断
					
						if(mCurrentCmd.isRecordInterval)//需要记录两次执行相同命令的间隔
							mCurrentCmd.pDevice->Updated();
						
						//检查队列是否为空
						if(mCmdQueue.Size()>0)//队列不为空
						{
							mCmdQueue.Get(mCurrentCmd);//下一个命令出队
							//都要先发送从机地址+写信号
							mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
							mIndex_Send = 0;//发送、接收计数下标清零
							I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
							I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号
						}
						else//队列为空，
						{
							mState=STATE_READY;//将状态设置为 准备好发送 模式
						}
					}
			}
			else
			{
					(*mCurrentCmd.pDataIn) = I2C_ReceiveData(mI2C);
					++mCurrentCmd.pDataIn;//下标后移
					--mCurrentCmd.inDataLen;//需要接收数据计数减一
					if(mCurrentCmd.inDataLen==1)//还剩最后一个字节需要接收
					{
						I2C_AcknowledgeConfig(mI2C,DISABLE);//关应答
						I2C_GenerateSTOP(mI2C,ENABLE);		//产生停止信号
					}
					else if(mCurrentCmd.inDataLen==0)//接收完毕
					{
						if(mCurrentCmd.isRecordInterval)//需要记录两次执行相同命令的间隔
							mCurrentCmd.pDevice->Updated();
						
						//检查队列是否为空
						if(mCmdQueue.Size()>0)//队列不为空
							{
								mCmdQueue.Get(mCurrentCmd);//下一个命令出队
								mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
								mIndex_Send = 0;//发送、接收计数下标清零
								I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
								I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号
							}
							else//队列为空，
							{
								mState=STATE_READY;//将状态设置为 准备好发送 模式
							}
					}
			}
		#else
			(*mCurrentCmd.pDataIn) = I2C_ReceiveData(mI2C);
			++mCurrentCmd.pDataIn;//下标后移
			--mCurrentCmd.inDataLen;//需要接收数据计数减一
			if(mCurrentCmd.inDataLen==1)//还剩最后一个字节需要接收
			{
				I2C_AcknowledgeConfig(mI2C,DISABLE);//关应答
				I2C_GenerateSTOP(mI2C,ENABLE);		//产生停止信号
			}
			else if(mCurrentCmd.inDataLen==0)//接收完毕
			{
				if(mCurrentCmd.recordIntervalNumber)//需要记录两次执行相同命令的间隔
					mCurrentCmd.pDevice->Updated();
				
				//检查队列是否为空
				if(mCmdQueue.Size()>0)//队列不为空
					{
						mCmdQueue.Get(mCurrentCmd);//下一个命令出队
						mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
						mIndex_Send = 0;//发送、接收计数下标清零
						I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
						I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号
					}
					else//队列为空，
					{
						mState=STATE_READY;//将状态设置为 准备好发送 模式
					}
			}
		#endif
		}
			break;
		
		
		case I2C_EVENT_MASTER_BYTE_TRANSMITTED:	//EV8_2					//已: I2C_SendData() 
		if(STATE_SEND_DATA==mState)//防止标志没来得及清掉，重复进入
		{
			#ifdef I2C_USE_DMA
			if(mUseDma)
			{
				if(0==DMA_GetCurrDataCounter(mDmaTxChannel) && mState == STATE_SEND_DATA)//检查是否发送完毕
				{
						DMA_Cmd(mDmaTxChannel, DISABLE);//关DMA
						if(mCurrentCmd.cmdType >= I2C_WRITE_BYTE)//处于写模式 单字节或者多字节
						{
							I2C_GenerateSTOP(mI2C,ENABLE);//产生停止信号
							
							if(mCurrentCmd.isRecordInterval)//需要记录两次执行相同命令的间隔
								mCurrentCmd.pDevice->Updated();
							
							//判断队列是否为空
							if(mCmdQueue.Size()>0)//队列不为空
							{
								mCmdQueue.Get(mCurrentCmd);//下一个命令出队
								
								 //都要先发送从机地址+写信号
								mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
								mIndex_Send = 0;//发送、接收计数下标清零
								I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
								I2C_GetLastEvent(mI2C);//！！这里再次获取一下状态的原因其实是起延时的作用，等待busy位复位
									//因为只有在busy位为0时才可以设置start位才有用，否则可能导致开始信号无法发送的情况
							/*	Delay::Us(1);   */
							/*	usart1<<"ok\n";  */
								I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号
							}
							else//队列为空，
							{
								
								mState=STATE_READY;//将状态设置为 准备好发送 模式
							}
						}
						else//处于读模式
						{
							mState = STATE_SEND_ADR;//状态设置为 发送从机地址+读 
							I2C_GenerateSTART(mI2C,ENABLE);//起始信号
						}
				}
			}
			else
			{
				if(mCurrentCmd.outDataLen>0)//还需要发送数据
				{
					I2C_SendData(mI2C,mCurrentCmd.DataOut[mIndex_Send++]);//发送寄存器地址或者命令
					--mCurrentCmd.outDataLen;//需要发送的数据长度减1
				}
				else//数据已经发送完成
				{
						if(mCurrentCmd.cmdType >= I2C_WRITE_BYTE)//处于写模式 单字节或者多字节
						{
							//已经写完，发送停止信号
							I2C_GenerateSTOP(mI2C,ENABLE);//产生停止信号
							if(mCurrentCmd.isRecordInterval)//需要记录两次执行相同命令的间隔
								mCurrentCmd.pDevice->Updated();
							//判断队列是否为空
							if(mCmdQueue.Size()>0)//队列不为空
							{
								mCmdQueue.Get(mCurrentCmd);//下一个命令出队
								mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
								mIndex_Send = 0;//发送、接收计数下标清零
								I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
								I2C_GetLastEvent(mI2C);//！！这里再次获取一下状态的原因其实是起延时的作用，等待busy位复位
									//因为只有在busy位为0时才可以设置start位才有用，否则可能导致开始信号无法发送的情况
								I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号
							}
							else//队列为空，
							{
								mState=STATE_READY;//将状态设置为 准备好发送 模式
							}
						}
						else//处于读模式
						{
								mState = STATE_SEND_ADR;//状态设置为 发送从机地址+读 
								I2C_GenerateSTART(mI2C,ENABLE);//起始信号
						}
				}
			}
			#else
				if(mCurrentCmd.outDataLen>0)//还需要发送数据
				{
					I2C_SendData(mI2C,mCurrentCmd.DataOut[mIndex_Send++]);//发送寄存器地址或者命令
					--mCurrentCmd.outDataLen;//需要发送的数据长度减1
				}
				else//数据已经发送完成
				{
						if(mCurrentCmd.cmdType >= I2C_WRITE_BYTE)//处于写模式 单字节或者多字节
						{
							//已经写完，发送停止信号
							I2C_GenerateSTOP(mI2C,ENABLE);//产生停止信号
							if(mCurrentCmd.isRecordInterval)//需要记录两次执行相同命令的间隔
								mCurrentCmd.pDevice->Updated();
							//判断队列是否为空
							if(mCmdQueue.Size()>0)//队列不为空
							{
								mCmdQueue.Get(mCurrentCmd);//下一个命令出队
								mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
								mIndex_Send = 0;//发送、接收计数下标清零
								I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
								I2C_GetLastEvent(mI2C);//！！这里再次获取一下状态的原因其实是起延时的作用，等待busy位复位
									//因为只有在busy位为0时才可以设置start位才有用，否则可能导致开始信号无法发送的情况
								I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号
							}
							else//队列为空，
							{
								mState=STATE_READY;//将状态设置为 准备好发送 模式
							}
						}
						else//处于读模式
						{
							
								mState = STATE_SEND_ADR;//状态设置为 发送从机地址+读 
								I2C_GenerateSTART(mI2C,ENABLE);//起始信号
						}
				}
			#endif
			}
			else
			{
				++timeOut;
				if(timeOut>3)
				{
					Soft_Reset();
					mState = STATE_ERROR;//状态设置为错误状态
				}
			}
			break;
		
//		case I2C_EVENT_MASTER_BYTE_TRANSMITTING://EV8   /* TRA, BUSY, MSL, TXE flags */
//			break;
		
		case 0x00030044://接收数据时出现数据溢出错误,DR为读出，
			I2C_ReceiveData(mI2C); //读出数据，清除BTF标志
			break;
		case 0x00000010: //STOPF ：从机收到停止信号，但本程序中并没有考虑从机模式，在这里判定为出错
			//（如果总线上没有另外的主机，可能是由于：当主机发送完数据后（主动产生停止信号后）自动进入从机模式
			//而此时比如sda和scl线接触不良，将导致产生错误的停止信号，从而出现此中断，故判定为断线，将iic设备关闭）
			Soft_Reset();
			mState = STATE_ERROR;//状态设置为错误状态
			break;
		case 0x00000040:
			Soft_Reset();
			mState = STATE_ERROR;//状态设置为错误状态
			break;
	}
	if(I2C_Status!=I2C_EVENT_MASTER_BYTE_TRANSMITTED)//
		timeOut=0;
	if((I2C_Status&0x00010000)==0)//从机模式，主机主动产生停止信号之后便会进入从机模式
	{
//		Soft_Reset();
//		mState = STATE_ERROR;//状态设置为错误状态
	}
}

void I2C::ErrorIRQ()
{
	u16 Error=mI2C->SR1 & ((uint16_t)0x0F00) ; /*!< I2C errors Mask  *///获取错误信息
	mI2C->SR1 = ~((uint16_t)0x0F00);//清除错误信息
	#ifdef DEBUG
			LOG("ER_IRQ\n");
		#endif
	/* If Bus error occurred ---------------------------------------------------*/
	if((Error&I2C_ERR_BERR)!=0)
	{
		#ifdef DEBUG
			LOG("BERR\n");
		#endif
			/* Generate I2C software reset in order to release SDA and SCL lines */
			mI2C->CR1 |= I2C_CR1_SWRST; 
			mI2C->CR1 &= ~I2C_CR1_SWRST;
	}
          
    /* If Arbitration Loss error occurred --------------------------------------*/
	if((Error&I2C_ERR_ARLO)!=0)
	{
		#ifdef DEBUG
			LOG("ARLO\n");
		#endif
			/* Generate I2C software reset in order to release SDA and SCL lines */
			mI2C->CR1 |= I2C_CR1_SWRST; 
			mI2C->CR1 &= ~I2C_CR1_SWRST; 
	}
    
    /* If Overrun error occurred -----------------------------------------------*/
	if((Error&I2C_ERR_OVR)!=0)
	{
		#ifdef DEBUG
			LOG("OVR\n");
		#endif
		  /* No I2C software reset is performed here in order to allow user to get back
		  the last data received correctly */	    
    }
        
    /* If Acknowledge Failure error occurred -----------------------------------*/
	if((Error&I2C_ERR_AF)!=0)
    {
		#ifdef DEBUG
			LOG("AF\n");
		#endif
		  /* No I2C software reset is performed here in order to allow user to recover 
		  communication */ 
    }   
	    /* USE_SINGLE_ERROR_CALLBACK is defined in cpal_conf.h file */
	#if defined(USE_ERROR_CALLBACK)  
		/* Call Error UserCallback */  
		I2C_Error_UserCallback(Error);
		I2C_ERR_UserCallback(Error);
	#endif /* USE_SINGLE_ERROR_CALLBACK */
	
	
	
	Soft_Reset();//软件复位，清除I2C所有寄存器中的值
	
	mState = STATE_ERROR;//状态设置为错误状态
//	IIC_CMD_Queue.State = STATE_READY;//状态设置为正常
}

#ifdef I2C_USE_DMA

void I2C::DmaTxIRQ()
{
	if((uint32_t)( DMA1->ISR & mDmaRxTcFlag )!=0)//传输完成
	{

	} /*
	else if((uint32_t)( DMA1->ISR & I2C_DMA_RX_HT_FLAG )!=0)//传输一半
	{
		
	}
	else if((uint32_t)( DMA1->ISR & DMA1_FLAG_TE6 )!=0)//传输发生错误
	{
		usart1<<"error TX DMA\n";
	}*/ 
	DMA1->IFCR=DMA1->ISR;//清除中断标志
}
void I2C::DmaRxIRQ()
{
	if((uint32_t)( DMA1->ISR & mDmaRxTcFlag )!=0)//传输完成
	{
	//	if(!DMA_GetCurrDataCounter(I2C_DMA_RX_Channel))//检查是否接收完毕   //此处不用判断 ,判断会出错， 为查明情况
		{
			DMA_Cmd(mDmaRxChannel, DISABLE);//关DMA
			I2C_GenerateSTOP(mI2C,ENABLE);		//产生停止信号  
								//检查队列是否为空
			if(mCmdQueue.Size()>0)//队列不为空
				{
					mCmdQueue.Get(mCurrentCmd);//下一个命令出队
					//都要先发送从机地址+写信号
					
					mState = STATE_SEND_ADW;//标志设置为需要发送从机地址+写信号
					mIndex_Send = 0;//发送、接收计数下标清零
					I2C_GetLastEvent(mI2C);//！！这里再次获取一下状态的原因其实是起延时的作用，等待busy位复位
								//因为只有在busy位为0时才可以设置start位才有用，否则可能导致开始信号无法发送的情况
					I2C_AcknowledgeConfig(mI2C,ENABLE);	//使能应答
					I2C_GenerateSTART(mI2C,ENABLE);		//产生启动信号	
				}
				else//队列为空，
				{
					mState=STATE_READY;//将状态设置为 准备好发送 模式
				}
		}
	} /*
	else if((uint32_t)( DMA1->ISR & I2C_DMA_RX_HT_FLAG )!=0)//传输一半
	{
		
	}
	else if((uint32_t)( DMA1->ISR & DMA1_FLAG_TE7 )!=0)//传输发生错误
	{
		usart1<<"error RX DMA\n";
	} */
	DMA1->IFCR=DMA1->ISR;//清除中断标志
}
#endif

I2C::I2C(u8 i2cNumber,bool useDMA,u32 speed,u8 remap,u8 priorityGroup,
					uint8_t preemprionPriorityEvent,uint8_t subPriorityEvent,
					uint8_t preemprionPriorityError,uint8_t subPriorityError,
					uint8_t preemprionPriorityDma,uint8_t subPriorityDma)
{
#ifdef USE_I2C
#ifndef USE_I2C1
#ifndef USE_I2C2
 return;
#endif	
#endif
#ifdef I2C_USE_DMA
	mUseDma=useDMA;
	if(mUseDma)
	{
		//iic DMA 固定的参数，后面无需修改的参数
		mDMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
		mDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		mDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		mDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte ;
		mDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		mDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		mDMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	}
#endif	
	//成员变量赋值
	mSpeed=speed;
	if(i2cNumber==1)//I2C1
	{
		#ifdef USE_I2C1
		mI2C=I2C1;
	#ifdef I2C_USE_DMA
		
		if(mUseDma)
		{
			mDmaTxChannel=DMA1_Channel6;
			mDmaRxChannel=DMA1_Channel7;
			mDmaRxTcFlag=DMA1_FLAG_TC7;
			mDmaTxTcFlag=DMA1_FLAG_TC6;
			
		}
	#endif
		#ifdef USE_I2C1
	  	pI2C1=this;
		#endif
		mRemap=remap;
		#endif
	}
	else//iic2
	{
		#ifdef USE_I2C2
		mI2C=I2C2;
	#ifdef I2C_USE_DMA
		if(mUseDma)
		{
			mDmaTxChannel=DMA1_Channel4;
			mDmaRxChannel=DMA1_Channel5;
			mDmaRxTcFlag=DMA1_FLAG_TC5;
			mDmaTxTcFlag=DMA1_FLAG_TC4;
		}
	#endif
		#ifdef USE_I2C2
			pI2C2=this;
		#endif
		mRemap=false;
		#endif
	}
	mPriority[0]=preemprionPriorityEvent;
	mPriority[1]=subPriorityEvent;
	mPriority[2]=preemprionPriorityError;
	mPriority[3]=subPriorityError;
	mPriority[4]=preemprionPriorityDma;
	mPriority[5]=subPriorityDma;
	
	
	//初始化
	Init();
		//中断初始化
	switch(priorityGroup)
	{
		case 0:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
			break;
		case 1:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
			break;
		case 2:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			break;
		default:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
			break;
		case 4:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
			break;
	}
  #ifdef I2C_USE_DMA
	if(mUseDma)
	{
		mDMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&(mI2C->DR);//外设地址，要在iic初始化完成之后设置，因为mI2c->DR未初始化
	}
  #endif
#endif
}
bool I2C::Init()
{
	I2C_InitTypeDef I2C_InitStructure;
	#ifdef I2C_USE_DMA
		DMA_InitTypeDef DMA_InitStructure;
		uint8_t dmaTxIrq,dmaRxIrq;
	#endif
	NVIC_InitTypeDef NVIC_InitStructure; 
	uint8_t eventIrq,errorIrq;
	uint32_t i2cClk;//i2c clock
	
	uint16_t sclPin,sdaPin;
	
	if(this->GetI2CNumber()==1)//i2c1
	{
		#ifdef USE_I2C1
		i2cClk=RCC_APB1Periph_I2C1;//i2 clock setting
		eventIrq=I2C1_EV_IRQn;
		errorIrq=I2C1_ER_IRQn;
		#ifdef I2C_USE_DMA
		if(mUseDma)
		{
			dmaTxIrq=DMA1_Channel6_IRQn;
			dmaRxIrq=DMA1_Channel7_IRQn;
		}
		#endif
		if(mRemap)//remap gpio pin
		{
			sclPin=GPIO_Pin_8;
			sdaPin=GPIO_Pin_9;
		}
		else
		{
			sclPin=GPIO_Pin_6;
			sdaPin=GPIO_Pin_7;
		}
		#endif
	}
	else
	{
		#ifdef USE_I2C2
		i2cClk=RCC_APB1Periph_I2C2;
		sclPin=GPIO_Pin_10;
		sdaPin=GPIO_Pin_11;
		eventIrq=I2C2_EV_IRQn;
		errorIrq=I2C2_ER_IRQn;
		#ifdef I2C_USE_DMA
		if(mUseDma)
		{
			dmaTxIrq=DMA1_Channel4_IRQn;
			dmaRxIrq=DMA1_Channel5_IRQn;
		}
		#endif
			pI2C2=this;
		#endif 
	}
	
	Soft_Reset();//清空I2C相关寄存器  //重要，不能删除
	
////设置成默认值
	mI2C->CR1 &= ~I2C_CR1_PE;//I2C失能  I2C_Cmd(I2C,DISABLE); 								//失能 I2C		
	
//	I2CGPIODeInit(sclPin,sdaPin);//IO设置成默认值
//	
//	RCC_APB1PeriphResetCmd(i2cClk,ENABLE);//重置clk时钟 防止有错误标志 // I2C_DeInit(I2C);  //将IIC端口初始化，否则GPIO不能被操作
//	RCC_APB1PeriphResetCmd(i2cClk,DISABLE);//关闭clk重置
//	RCC_APB1PeriphClockCmd(i2cClk,DISABLE);//关闭CLK时钟
	#ifdef I2C_USE_DMA
	if(mUseDma)
	{
		//dma默认值  重要，不能去掉，不然在硬件连接有错时可能会导致DMA无法恢复，以致不能使用DMA发送数据
		DMA_DeInit(mDmaTxChannel);
		DMA_DeInit(mDmaRxChannel);
	}
	#endif
/////初始化	
	//I2C CLK初始化
	RCC_APB1PeriphResetCmd(i2cClk,ENABLE);//重置clk时钟 防止有错误标志
	RCC_APB1PeriphResetCmd(i2cClk,DISABLE);//关闭clk重置
	RCC_APB1PeriphClockCmd(i2cClk,ENABLE);//开启I2C时钟
	
	if(!I2C_CHACK_BUSY_FIX(i2cClk,sclPin,sdaPin))//检测总线是否被从机拉低（SDA为低）（一般是因为传送过程中出错导致无法一直处于busy状态）
	{
		mState=STATE_ERROR;
		return 0;
	}
	
	I2CGPIOInit(sclPin,sdaPin);
  
	//i2c使能 I2C_Cmd(I2C,ENABLE); 
	//mI2C->CR1 |= I2C_CR1_PE;//由于在下面I2C_Init()函数中会开启，所以这里屏蔽了
	#ifdef I2C_USE_DMA
	if(mUseDma)
	{
		//使能DMA
		mI2C->CR2 |= I2C_CR2_DMAEN;
	}
	#endif
	I2C_InitStructure.I2C_ClockSpeed          = mSpeed;                        //Initialize the I2C_ClockSpeed member
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;                  // Initialize the I2C_Mode member 
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;               // Initialize the I2C_DutyCycle member 
	I2C_InitStructure.I2C_OwnAddress1         = 0;                             // Initialize the I2C_OwnAddress1 member 
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;                // Initialize the I2C_Ack member 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  // Initialize the I2C_AcknowledgedAddress member 
  
	I2C_Init(mI2C,&I2C_InitStructure);//iic初始化
	

	#ifdef I2C_USE_DMA
	if(mUseDma)
	{
		//DMA初始化
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
		// I2Cx Common Channel Configuration 
		DMA_InitStructure.DMA_BufferSize = 0;
		DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte ;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// Select I2Cx DR Address register as DMA PeripheralBaseAddress 
		DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(mI2C->DR);
		// Select Memory to Peripheral transfer direction 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_Init(mDmaTxChannel,&DMA_InitStructure);
		// Select Peripheral to Memory transfer direction 
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_Init(mDmaRxChannel,&DMA_InitStructure);
	}
	#endif
	
	

	// Enable the IRQ channel 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	// Configure NVIC for I2Cx EVT Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = eventIrq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = mPriority[0];
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = mPriority[1];
	NVIC_Init(&NVIC_InitStructure);
	mI2C->CR2 |= I2C_CR2_ITEVTEN;//                            开启I2C事件中断
 // Configure NVIC for I2Cx ERR Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = errorIrq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = mPriority[2];
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = mPriority[3];
    NVIC_Init(&NVIC_InitStructure);
	mI2C->CR2 |= I2C_CR2_ITERREN;//                             开启I2C错误中断
	#ifdef I2C_USE_DMA
	if(mUseDma)
	{
		mI2C->CR2 &= ~I2C_IT_BUF; //使用DMA模式时勿打开 BUF中断
	}
	else//开启了dma开关，但是选择不使用dma功能
	{
		mI2C->CR2 |= I2C_IT_BUF; //使用中断模式打开BUF中断
	}
	#else
	mI2C->CR2 |= I2C_IT_BUF; //使用中断模式打开BUF中断
	#endif
	#ifdef I2C_USE_DMA
	if(mUseDma)
	{
		// Configure NVIC for DMA TX channel interrupt 
		NVIC_InitStructure.NVIC_IRQChannel = dmaTxIrq ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = mPriority[4];
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = mPriority[5];
		NVIC_Init(&NVIC_InitStructure);
		// Enable DMA TX Channel TCIT  
		mDmaTxChannel->CCR |= DMA_IT_TC;  //打开发送完成中断
		// Enable DMA TX Channel TEIT  
		mDmaTxChannel->CCR |= DMA_IT_TE; //打开错误中断
		// Enable DMA TX Channel HTIT  
		//I2C_DMA_TX_Channel->CCR |= DMA_IT_HT;
		// Configure NVIC for DMA RX channel interrupt 
		NVIC_InitStructure.NVIC_IRQChannel = dmaRxIrq ;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = mPriority[4];
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = mPriority[5];
		NVIC_Init(&NVIC_InitStructure);
		// Enable DMA TX Channel TCIT  
		mDmaRxChannel->CCR |= DMA_IT_TC; //打开接收完成中断
		// Enable DMA TX Channel TEIT  
		mDmaRxChannel->CCR |= DMA_IT_TE; //打开接收错误中断
		// Enable DMA TX Channel HTIT  
		//I2C_DMA_RX_Channel->CCR |= DMA_IT_HT;	 
	}
	#endif	
	mState=STATE_READY;//队列状态设置为可以开始下一个任务
	return 1;
}
/*
bool I2C::Init(u8 i2cNumber,u32 speed,u8 remap,u8 priorityGroup,
					uint8_t preemprionPriorityEvent,uint8_t subPriorityEvent,
					uint8_t preemprionPriorityError,uint8_t subPriorityError,
					uint8_t preemprionPriorityDma,uint8_t subPriorityDma)
{
	I2C_InitTypeDef I2C_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	uint8_t eventIrq,errorIrq,dmaTxIrq,dmaRxIrq;
	uint32_t i2cClk;//i2c clock
	
	uint16_t sclPin,sdaPin;
	mSpeed=speed;
	mPriority[0]=priorityGroup;
	mPriority[1]=preemprionPriorityEvent;
	mPriority[2]=subPriorityEvent;
	mPriority[3]=preemprionPriorityError;
	mPriority[4]=subPriorityError;
	mPriority[5]=preemprionPriorityDma;
	mPriority[6]=subPriorityDma;
	if(i2cNumber==1)
	{
		mI2C=I2C1;
		i2cClk=RCC_APB1Periph_I2C1;//i2 clock setting
		mDmaTxChannel=DMA1_Channel6;
		mDmaRxChannel=DMA1_Channel7;
		eventIrq=I2C1_EV_IRQn;
		errorIrq=I2C1_ER_IRQn;
		dmaTxIrq=DMA1_Channel6_IRQn;
		dmaRxIrq=DMA1_Channel7_IRQn;
		mDmaRxTcFlag=DMA1_FLAG_TC7;
		mDmaTxTcFlag=DMA1_FLAG_TC6;
		pI2C1=this;
		if(remap)//remap gpio pin
		{
			sclPin=GPIO_Pin_8;
			sdaPin=GPIO_Pin_9;
			mRemap=0x01;
		}
		else
		{
			sclPin=GPIO_Pin_6;
			sdaPin=GPIO_Pin_7;
		}
	}
	else
	{
		mI2C=I2C2;
		i2cClk=RCC_APB1Periph_I2C2;
		mDmaTxChannel=DMA1_Channel4;
		mDmaRxChannel=DMA1_Channel5;
		sclPin=GPIO_Pin_10;
		sdaPin=GPIO_Pin_11;
		eventIrq=I2C2_EV_IRQn;
		errorIrq=I2C2_ER_IRQn;
		dmaTxIrq=DMA1_Channel4_IRQn;
		dmaRxIrq=DMA1_Channel5_IRQn;
		mDmaRxTcFlag=DMA1_FLAG_TC5;
		mDmaTxTcFlag=DMA1_FLAG_TC4;
		pI2C2=this;
	}
	
	Soft_Reset();//清空I2C相关寄存器  //重要，不能删除
	
////设置成默认值
	mI2C->CR1 &= ~I2C_CR1_PE;//I2C失能  I2C_Cmd(I2C,DISABLE); 								//失能 I2C		
	
//	I2CGPIODeInit(sclPin,sdaPin);//IO设置成默认值
//	
//	RCC_APB1PeriphResetCmd(i2cClk,ENABLE);//重置clk时钟 防止有错误标志 // I2C_DeInit(I2C);  //将IIC端口初始化，否则GPIO不能被操作
//	RCC_APB1PeriphResetCmd(i2cClk,DISABLE);//关闭clk重置
//	RCC_APB1PeriphClockCmd(i2cClk,DISABLE);//关闭CLK时钟
	
	//dma默认值  重要，不能去掉，不然在硬件连接有错时可能会导致DMA无法恢复，以致不能使用DMA发送数据
	DMA_DeInit(mDmaTxChannel);
	DMA_DeInit(mDmaRxChannel);
	
/////初始化	
	//I2C CLK初始化
	RCC_APB1PeriphResetCmd(i2cClk,ENABLE);//重置clk时钟 防止有错误标志
	RCC_APB1PeriphResetCmd(i2cClk,DISABLE);//关闭clk重置
	RCC_APB1PeriphClockCmd(i2cClk,ENABLE);//开启I2C时钟
	
	if(!I2C_CHACK_BUSY_FIX(i2cClk,sclPin,sdaPin))//检测总线是否被从机拉低（SDA为低）（一般是因为传送过程中出错导致无法一直处于busy状态）
	{
		mState=STATE_ERROR;
		return 0;
	}
	
	I2CGPIOInit(sclPin,sdaPin);
  
	//i2c使能 I2C_Cmd(I2C,ENABLE); 
	//mI2C->CR1 |= I2C_CR1_PE;//由于在下面I2C_Init()函数中会开启，所以这里屏蔽了
	
	//使能DMA
	mI2C->CR2 |= I2C_CR2_DMAEN;
	
	I2C_InitStructure.I2C_ClockSpeed          = mSpeed;                        //Initialize the I2C_ClockSpeed member
	I2C_InitStructure.I2C_Mode                = I2C_Mode_I2C;                  // Initialize the I2C_Mode member 
	I2C_InitStructure.I2C_DutyCycle           = I2C_DutyCycle_2;               // Initialize the I2C_DutyCycle member 
	I2C_InitStructure.I2C_OwnAddress1         = 0;                             // Initialize the I2C_OwnAddress1 member 
	I2C_InitStructure.I2C_Ack                 = I2C_Ack_Enable;                // Initialize the I2C_Ack member 
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  // Initialize the I2C_AcknowledgedAddress member 
  
	I2C_Init(mI2C,&I2C_InitStructure);//iic初始化
	

	
	//DMA初始化
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	// I2Cx Common Channel Configuration 
	DMA_InitStructure.DMA_BufferSize = 0;
	DMA_InitStructure.DMA_PeripheralInc =  DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte ;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// Select I2Cx DR Address register as DMA PeripheralBaseAddress 
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(mI2C->DR);
	// Select Memory to Peripheral transfer direction 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(mDmaTxChannel,&DMA_InitStructure);
	// Select Peripheral to Memory transfer direction 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Init(mDmaRxChannel,&DMA_InitStructure);
	
	
	
	//中断初始化
	switch(priorityGroup)
	{
		case 0:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
			break;
		case 1:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
			break;
		case 2:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			break;
		default:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
			break;
		case 4:
			NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
			break;
	}
	// Enable the IRQ channel 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	// Configure NVIC for I2Cx EVT Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = eventIrq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemprionPriorityEvent;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriorityEvent;
	NVIC_Init(&NVIC_InitStructure);
	mI2C->CR2 |= I2C_CR2_ITEVTEN;//                            开启I2C事件中断
 // Configure NVIC for I2Cx ERR Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = errorIrq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemprionPriorityError;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriorityError;
    NVIC_Init(&NVIC_InitStructure);
	mI2C->CR2 |= I2C_CR2_ITERREN;//                             开启I2C错误中断
	
	mI2C->CR2 &= ~I2C_IT_BUF; //使用DMA模式时勿打开 BUF中断

	// Configure NVIC for DMA TX channel interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = dmaTxIrq ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemprionPriorityDma;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriorityDma;
	NVIC_Init(&NVIC_InitStructure);
	// Enable DMA TX Channel TCIT  
	mDmaTxChannel->CCR |= DMA_IT_TC;  //打开发送完成中断
	// Enable DMA TX Channel TEIT  
	mDmaTxChannel->CCR |= DMA_IT_TE; //打开错误中断
	// Enable DMA TX Channel HTIT  
	//I2C_DMA_TX_Channel->CCR |= DMA_IT_HT;
	// Configure NVIC for DMA RX channel interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = dmaRxIrq ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemprionPriorityDma;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subPriorityDma;
	NVIC_Init(&NVIC_InitStructure);
	// Enable DMA TX Channel TCIT  
	mDmaRxChannel->CCR |= DMA_IT_TC; //打开接收完成中断
	// Enable DMA TX Channel TEIT  
	mDmaRxChannel->CCR |= DMA_IT_TE; //打开接收错误中断
	// Enable DMA TX Channel HTIT  
	//I2C_DMA_RX_Channel->CCR |= DMA_IT_HT;	  
	mState=STATE_READY;//队列状态设置为可以开始下一个任务
	return 1;
}
*/

/////////////////////////
///重置I2C总线状态
/////////////////////////
void I2C::Soft_Reset()
{
	mI2C->CR1 |= I2C_CR1_SWRST; 
	mI2C->CR1 &= ~I2C_CR1_SWRST;
}

void I2C::I2CGPIODeInit(uint16_t sclPin,uint16_t sdaPin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Set GPIO frequency to 50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Select Input floating mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	GPIO_InitStructure.GPIO_Pin = sclPin;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Deinitialize I2Cx SDA Pin */
	GPIO_InitStructure.GPIO_Pin =sdaPin;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void I2C::I2CGPIOInit(uint16_t sclPin,uint16_t sdaPin)
{  
	//IO初始化
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启scl  sda时钟


	/* Enable Pin Remap if PB8 (SCL) and PB9 (SDA) is used for I2C1 复用打开 */
	if ( mI2C==I2C1 && mRemap )
	{
		/* Enable GPIO Alternative Functions */
		RCC_APB2PeriphClockCmd((RCC_APB2Periph_AFIO),ENABLE);
		/* Enable I2C1 pin Remap */
		GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	}

	/* Set GPIO frequency to 50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* Select Output open-drain mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
									
	/* Initialize I2Cx SCL Pin */ 
	GPIO_InitStructure.GPIO_Pin = sclPin;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Initialize I2Cx SDA Pin */
	GPIO_InitStructure.GPIO_Pin = sdaPin;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}


///////////////////////////////
///检测总线是否为busy，若是，进行修复
///@retval -1:成功 -0:失败
///////////////////////////////
bool I2C::I2C_CHACK_BUSY_FIX(uint32_t i2cClock,uint16_t sclPin,uint16_t sdaPin)
{
	u8 Time_out=0;
	GPIO_InitTypeDef GPIO_InitStructure;

	while(I2C_GetFlagStatus(mI2C, I2C_FLAG_BUSY)&&Time_out<20)
	{
		RCC_APB1PeriphClockCmd(i2cClock,DISABLE);//开启I2C时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启scl  sda时钟


		/* Set GPIO frequency to 50MHz */
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

		/* Select Output open-drain mode */
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
									
		/* Initialize I2Cx SCL Pin */ 
		GPIO_InitStructure.GPIO_Pin = sclPin;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		/* Initialize I2Cx SDA Pin */
		GPIO_InitStructure.GPIO_Pin = sdaPin;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
		  
		//模拟方式产生停止信号
		GPIO_ResetBits(GPIOB, sclPin);
		GPIO_ResetBits(GPIOB, sdaPin);
		DelayUs(5);
		GPIO_SetBits(GPIOB, sclPin);
		DelayUs(5);
		GPIO_SetBits(GPIOB, sdaPin);
		DelayUs(5);
		
		I2CGPIODeInit(sclPin,sdaPin);//IO设置成默认值
		RCC_APB1PeriphResetCmd(i2cClock,ENABLE);//重置clk时钟 防止有错误标志
		RCC_APB1PeriphResetCmd(i2cClock,DISABLE);//关闭clk重置
		RCC_APB1PeriphClockCmd(i2cClock,ENABLE);//开启I2C时钟
		++Time_out;

	}
	if(Time_out==20)
		return 0;
	return 1;
}


void I2C::DelayUs(u32 nus)
{
	Delay::Us(nus);
}
void I2C::DelayMs(u16 nms)
{
	Delay::Ms(nms);
}
void I2C::DelayS(u32 ns)
{
	Delay::S(ns);
}

////////////////////////////
///return the number of iic
///@retval return the number of iic
/////////////////////////////
u8 I2C::GetI2CNumber()
{
	if( mI2C==I2C2)
		return 2;
	else
		return 1;
}

