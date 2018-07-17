/*
PA11  CANRX
PA12  CANTX
*/

#ifndef _CAN_H_
#define _CAN_H_

#include "stm32f10x.h"
#include "GPIO.h"
#include "stm32f10x_can.h"
#include "FIFOBuffer.h"

#define CAN_RX_BUFF 20    //接收buffer长度

class CAN{
  private:  
    FIFOBuffer<u8,CAN_RX_BUFF>  buffer_R;	//定义接收buferr
	  //中断方式
		void NVIC_init();
    void init();   
    bool mUseNvic; //是否使用中断方式	
	  uint8_t mSjw;//定义了重新同步跳跃宽度(SJW)，即在每位中可以延长或缩短多少个时间单位的上限
	  uint8_t mBs1;
	  uint8_t mBs2;
	  uint8_t mMode;//模式
	  uint8_t mPrescaler;
	
	public:
	  //参数： mode :  1回环模式  0：正常模式
	  //波特率 = (72000/2) / mPrescaler / (BS1 +BS2 +1)  36M/((1+8+7)*5)=450Kbp
	  //是否使用中断方式
   	CAN(uint8_t mode=1,uint8_t bs1 =8 ,uint8_t bs2 =7 ,uint8_t pre =5 ,bool useNvic = true);
    void Irq();
	  u8 Can_Send_Msg(u8* msg,u8 len);//发送
	  u16 ReceivedBufferSize();//获取buffer长度
	  bool GetReceivedData(u8* buffer,u16 number);//取出数据
	  void ClearReceiveBuffer();//清空接收buffer
	  u8 Can_Receive_Msg(u8 *buf);//查询方式读取
}; 
#endif
