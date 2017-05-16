/**
*@file FIFOBuffer.h
*@author cq_dan3  2015-10-20 first creat FIFOBuffer class
*        Neutree  2015-10-28 add comments for FIFOBuffer class
*@version v1.1
*@brief A template for FIFO queue
*@copyright CQUTIOTLIB all right reserved
*
*/

#ifndef _FIFO_BUFFER_H_
#define _FIFO_BUFFER_H_

#include "stm32f10x.h"


/**
  *@breif A FIFO queue Template for FIFO application
  *
  *
  */
template<typename T, u16 MAX_SIZE>
class FIFOBuffer
{
private:
	T mData[MAX_SIZE];  //数据储存
	u16 mFront;         //头指针
	u16 mRear;          //尾指针
	//注意，没有使用长度这个变量，是为了防止有中断打断时快速反应（用mfront和mrear计算）
	//在put和get函数中体会，所以函数都是先操作头指针和为指针，再进行存数
public:

	/////////////////////////
	///@breif constructor
	/////////////////////////
	FIFOBuffer(); 
	
	
	/////////////////////////
	///@breif put node data to queue 
	///@param data the node data will put into queue
	////////////////////////
	bool Put(const T &data); 
	
	
	////////////////////////
	///@breif get one node data from queue,the length of queue will decrease one
	///@param data the destination of node data from queue
	////////////////////////
	bool Get(T &data);
	
	
	//////////////////////////////
	///@breif put multiple node data to queue 
	///@param pdata the first node data adress of node data array will put into queue
	///@param num the length of node data that will put into queue
	/////////////////////////////
	bool Puts(T *pData, u16 num); 
	
	
	//////////////////////////////
	///@breif get multiple node data from queue
	///@param pdata the first node data adress of node data array will get from the queue
	///@param num the length of node data that will get from the queue
	/////////////////////////////
	bool Gets(T *pData, u16 num);
	
	/**
	 * query first data from the queue, and the data remain be a part of the queue
	 * @param data the reference of the data that will be query
	 * @retval Is query succeed
	 */
	bool Query(T &data);

	/**
	 * query some data from the queue, and the data remain be a part of the queue
	 * @param data the address of the data save to
	 * @param startPosition Index of where to start query
	 * #param num The number of data will be query
	 * @retval Is query succeed
	 */
	bool QueryMul(T *data,u16 startPosition,u16 num);

	////////////////////////////
	///@breif get the size of queue
	///@retval the size of queue
	///////////////////////////
	u16 Size();
	
	
	////////////////////////////////
	///@breif clear the queue
	////////////////////////////////
	void Clear();
};

/**
 * Constructor,Initialize the queue
 */
template<typename T, u16 MAX_SIZE>
FIFOBuffer<T,MAX_SIZE>::FIFOBuffer()
:mFront(0),mRear(0)
{
//	for(u16 i=0;i<MAX_SIZE;++i)
//		mData[i]=0;
}

/**
 * input one node to buffer
 */
template<typename T, u16 MAX_SIZE>
bool FIFOBuffer<T,MAX_SIZE>::Put(const T &data) 
{
	if((mRear+1)%MAX_SIZE == mFront)//队满
		return false;
	else
	{
		mRear = (mRear+1)%MAX_SIZE;
		mData[mRear] = data;
	}
	return true;
}


/**
 * input multi nodes to buffer
 */
template<typename T, u16 MAX_SIZE>
bool FIFOBuffer<T,MAX_SIZE>::Puts(T *pData, u16 num) 
{
	if((mRear+num)%MAX_SIZE == mFront)//队满
		return false;
	for(u16 i=0;i<num;++i)
	{
		mRear = (mRear+1)%MAX_SIZE;
		mData[mRear] = pData[i];
	}
	return true;
}


/**
 * get one node from buffer
 */
template<typename T, u16 MAX_SIZE>
bool FIFOBuffer<T,MAX_SIZE>::Get(T &data) 
{
	if(mFront == mRear)
		return false;
	mFront = (mFront+1)%MAX_SIZE;
	data = mData[mFront];
	return true;
}


/**
 * get multi nodes from buffer
 */
template<typename T, u16 MAX_SIZE>
bool FIFOBuffer<T,MAX_SIZE>::Gets(T *pData, u16 num) 
{
	if(Size()<num)
		return false;

	for(u16 i=0;i<num;++i)
	{
		mFront = (mFront+1)%MAX_SIZE;
		pData[i] = mData[mFront];
	}
	return true;
}

/**
 * Get the number of nodes in buffer (_size)
 */
template<typename T, u16 MAX_SIZE>
u16 FIFOBuffer<T,MAX_SIZE>::Size()
{
	return (mRear - mFront+MAX_SIZE)%MAX_SIZE;
}

/**
 *clear all nodes in buffer
 */
template<typename T, u16 MAX_SIZE>
void FIFOBuffer<T,MAX_SIZE>::Clear()
{
	mFront = 0;
	mRear = 0;
//	for(u16 i=0;i<MAX_SIZE;++i)
//		mData[i]=0;
}
#endif
