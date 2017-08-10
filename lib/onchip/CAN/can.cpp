#include "can.h"
#include "Interrupt.h"
/* 
  CAN_Mode_Normal          正常模式
  CAN_Mode_LoopBack        环回模式
  CAN_Mode_Silent          静默模式
  CAN_Mode_Silent_LoopBack 静默环回模式  
*/

/*
CAN_SJW 定义了重新同步跳跃宽度(SJW)，即在每位中可以延长或缩短多少个时间单位的上限
CAN_SJW_1tq 重新同步跳跃宽度 1 个时间单位
CAN_SJW_2tq 重新同步跳跃宽度 2 个时间单位
CAN_SJW_3tq 重新同步跳跃宽度 3 个时间单位
CAN_SJW_4tq 重新同步跳跃宽度 4 个时间单位

*/

/*
CAN_BS1 设定了时间段 1 的时间单位数目
CAN_BS1_1tq 时间段 1 为 1 个时间单位
… …
CAN_BS1_16tq 时间段 1 为 16 个时间单位
*/

/*
CAN_BS2 设定了时间段 1 的时间单位数目
CAN_BS2_1tq 时间段 2 为 1 个时间单位
… …
CAN_BS2_8tq 时间段 2 为 8 个时间单位
*/

/*
波特率 = (72000/2) / mPrescaler / (BS1 +BS2 +1)
*/

void CAN::NVIC_init()
{
	NVIC_InitTypeDef  NVIC_InitStructure;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // 主优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void CAN::init()
{

	 //GPIO 时钟初始化
   GPIO_InitTypeDef GPIO_InitStructure; 
	 CAN_InitTypeDef CAN_InitStructure;
	 CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
   GPIO_Init(GPIOA, &GPIO_InitStructure);		//初始化IO
   
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
   GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化IO
	
		 
	 //CAN初始化	 
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
		
	CAN_InitStructure.CAN_TTCM=DISABLE;						 //非时间触发通信模式  //
	CAN_InitStructure.CAN_ABOM=DISABLE;						 //软件自动离线管理	 //
	CAN_InitStructure.CAN_AWUM=DISABLE;						 //睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)//
	CAN_InitStructure.CAN_NART=DISABLE;						 //禁止报文自动传送 //
	CAN_InitStructure.CAN_RFLM=DISABLE;						 //报文不锁定,新的覆盖旧的 // 
	CAN_InitStructure.CAN_TXFP=DISABLE;						 //优先级由报文标识符决定 //
	CAN_InitStructure.CAN_Mode= mMode;   //模式设置： mode:0,普通模式;1,回环模式; //
		
		  	//设置波特率
	CAN_InitStructure.CAN_SJW=mSjw;				   //重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=mBs1;         //Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=mBs2;         //Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=mPrescaler;    //CAN_Prescaler 设定了一个时间单位的长度，它的范围是 1 到 1024
	CAN_Init(CAN1, &CAN_InitStructure);       //初始化CAN1 
	
	 //过滤器初始化	
	CAN_FilterInitStructure.CAN_FilterNumber=0;	  //过滤器0  范围0-13
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; // CAN_FilterMode_IdMask 标识符屏蔽位模式   CAN_FilterMode_IdList 标识符列表模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; // 过滤器位宽 CAN_FilterScale_Two16bit 2 个 16 位过滤器  CAN_FilterScale_One32bit 1 个 32 位过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID  设定过滤器标识符
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK 用来设定过滤器屏蔽标识符或者过滤器标识符
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器0
   
	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0|CAN_IT_TME,ENABLE);
	//CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.	
	if(mUseNvic)
	{
		pCAN = this;
		NVIC_init();
	}
	
}

CAN::CAN(uint8_t mode,uint8_t bs1,uint8_t bs2,uint8_t pre ,bool useNvic)
{
	//数据初始化
	
	mUseNvic =useNvic; //是否使用中断方式	
	mSjw =  CAN_SJW_1tq;//定义了重新同步跳跃宽度(SJW)，即在每位中可以延长或缩短多少个时间单位的上限
	mBs1 =  bs1;  
	mBs2 =  bs2;
	mMode = mode; 
	mPrescaler = pre;  
	init();
	buffer_R.Clear();
}  

//中断服务函数			    
void CAN::Irq(void)
{
   CanRxMsg RxMessage;
   CAN_Receive(CAN1, 0, &RxMessage);
	 buffer_R.Puts(RxMessage.Data,RxMessage.DLC);//将接收到的数据存入buffer
}


void CAN::ClearReceiveBuffer()
{
	buffer_R.Clear();
}

//读取buffer长度
u16 CAN::ReceivedBufferSize()
{
	return buffer_R.Size();
}

//输出数据
bool CAN::GetReceivedData(u8* buffer,u16 number)
{
	if(buffer_R.Size()<number)//没有足够长的数据
		return false;
	else
	{
		buffer_R.Gets(buffer,number);//数据出队
		return true;
	}
}

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN::Can_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x00;					 // 标准标识符 
  //TxMessage.ExtId=0x00;				   // 设置扩展标示符 
  TxMessage.IDE=CAN_Id_Standard; // 标准帧
  TxMessage.RTR=CAN_RTR_Data;		 // 数据帧
  TxMessage.DLC=len;					 	// 要发送的数据长度
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)
		return 1;
  return 0;		
}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN::Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据	
    for(i=0;i<8;i++)
       buf[i]=RxMessage.Data[i];  
	  return RxMessage.DLC;	
}
