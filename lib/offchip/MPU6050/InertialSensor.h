#ifndef _INERTIAL_SENSOR_H_
#define _INERTIAL_SENSOR_H_
#include "Sensor.h"
#include "Vector3.h"

class InertialSensor
{
public:
	
	//////////////////////////
	///IMU初始化函数
	///@param wait if wait until the command in queue execute complete
	///@retval -1:检测到硬件 -2:检测到部分硬件 -3:正在检测 -0:未检测到硬件
	//////////////////////////
	virtual u8 Init(bool wait=false) =0 ;//初始化IMU


	////////////////////////////////
	///获取加速度原始值
	///@retval 返回三轴的加速度值
	///////////////////////////////
	virtual Vector3<int> GetAccRaw() =0 ;//获取加速度原始值


	////////////////////////////////
	///获取角速度原始值
	///@retval 返回三轴的角速度值 
	///////////////////////////////
	virtual	Vector3<int> GetGyrRaw() =0 ;//获取角速度原始值

	////////////////////////////////
	///获取角速度值
	///@retval 返回三轴的角速度值 单位：rad/s
	///////////////////////////////
	virtual	Vector3f GetGyr() =0 ;//获取角速度值
		
	////////////////////////////////
	///获取加速度值
	///@retval 返回三轴的加速度值  单位：g
	///////////////////////////////
	virtual	Vector3f GetAcc() =0 ;//获取加速度值
		
	///////////////////////////
	///角速度校准值
	//////////////////////////
	virtual Vector3<int> GetGyrOffset() =0 ;
	
	/////////////////////////////
	///角速度校准是否完毕
	//////////////////////////////
	virtual bool IsGyroCalibrated() =0 ;
	
	//////////////////////////////
	///从硬件更新加速度、角速度值到内存
	///@param wait if wait until the command in queue execute complete
	///@param acc The address of raw accelerator data save to 
	///@param gyro The address of raw gyro data save to
	///@return if wait set to true,MOD_READY:update succed MOD_ERROR:update fail  MOD_BUSY:Update interval is too short
	///        if wait set to false,MOD_ERROR:发送更新数据失败 MOD_READY:命令将会发送（具体的发送时间取决于队列中的排队的命令的数量）MOD_BUSY:Update interval is too short
	//////////////////////////////
	virtual u8 Update(bool wait=false,Vector3<int> *acc=0, Vector3<int> *gyro=0) =0 ;
	
	////////////////////////
	///开始角速度校准
	///@attention 校准时不要移动传感器
	/////////////////////////
	virtual void StartGyroCalibrate() =0 ;
	
	////////////////////////
	///停止角速度校准
	///@attention 校准时不要移动传感器
	/////////////////////////
	virtual void StopGyroCalibrate() =0 ;
	
	
	////////////////////
	///是否正在校准角速度
	///////////////////////
	virtual bool IsGyroCalibrating() =0 ;

	//////////////////////////////////////
	///获得两次数据采集的间隔
	//////////////////////////////////////
	virtual double GetUpdateInterval() =0 ;
};

#endif
