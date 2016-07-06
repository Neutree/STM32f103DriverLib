#ifndef __MPU6050_H
#define __MPU6050_H
/**
  *@file mpu6050.h
  *@breif mpu6050 lib   ,   and now only based on stm32 platform
  *  how to use:
  *      * import lib files(I2C FIFBuffer Delay Interrupt TaskManager mpu6050) to your project 
  *      * config your project 
  *         * decide to comment or umcomment #define I2C_USE_DMA  in I2C.h
  *             (if you don't want to use DMA, and also want to make code size smaller, just comment it)
  *         * comment # define   MPU6050_USE_TASKMANAGER in mpu6050.h or not, if you want to limit max update frequency, just uncomment it 
  *              if you uncomment it, and check Delay.h comment #define       DELAY_USE_SYSTICK  in Delay.h (because TaskManager used sysTick,Delay can not use it any more)
  *             (if you want limit the max update frequency umcomment it , or comment it)
  *      * create a iic object 
  *      * create a mpu6050 object , use iic object as mpu6050 constructor parameter
  *      * use Init() to initialize mpu6050 device
  *      * use Update() to update Data from device to memory
  *@author neucrack CQUT IOT lib
  *
  *
  *
  *
  *
  */

/**************************configuration****************************/
          # define   MPU6050_USE_TASKMANAGER

/*******************************************************************/


#include "stm32f10x.h"
#include "I2C.h"
#include "Vector3.h"
#include "InertialSensor.h"
#ifdef MPU6050_USE_TASKMANAGER
	#include "TaskManager.h"
#else
	#define	MOD_ERROR  0x00
	#define	MOD_READY  0x01
	#define	MOD_BUSY   0x02
#endif

typedef struct
{	
	//加速度
	u8 acc_XH;			//加速度 x轴 高字节
	u8 acc_XL;			//加速度 x轴 低字节
	u8 acc_YH;			//加速度 y轴 高字节
	u8 acc_YL;			//加速度 y轴 低字节
	u8 acc_ZH;			//加速度 z轴 高字节
	u8 acc_ZL;			//加速度 z轴 低字节	
		//温度
	u8 imu_TH;			//MPU6050温度 高字节
	u8 imu_TL;			//MPU6050温度 低字节
	//陀螺仪
	u8 gyro_XH;			//陀螺仪 x轴 高字节
	u8 gyro_XL;			//陀螺仪 x轴 低字节
	u8 gyro_YH;			//陀螺仪 y轴 高字节
	u8 gyro_YL;			//陀螺仪 y轴 低字节
	u8 gyro_ZH;			//陀螺仪 z轴 高字节
	u8 gyro_ZL;			//陀螺仪 z轴 低字节		
}Mpu6050Data;



class mpu6050:public InertialSensor,public Sensor{
private:
	I2C *mI2C;
	/*HealthState mHealth;//0:死亡  1：正常  2：连接正常，但是未初始化*/
	u8 mWhoAmI;
#ifdef MPU6050_USE_TASKMANAGER
	u16 mMaxUpdateFrequency;
#endif
	Mpu6050Data mData;

	bool mIsGyrCalibrating;      //角速度校准
	Vector3<int> mGyroOffset;    //角速度误差校准值
	Vector3<int> mGyroRaw;       //角速度原始值
	bool mIsGyrCalibrated;       //加速度是否已经校准完毕
public:


#ifdef MPU6050_USE_TASKMANAGER
	////////////////////////////
	///构造函数
	///@param i2c i2c对象指针
	///@param maxUpdateFrequency(Hz) The max update frequency (Hz), 
	///      if the frequency of call Update() more than maxUpdateFrequency, it will update as the maxUpdateFrequency
	////////////////////////////
		mpu6050(I2C &i2c,u16 maxUpdateFrequency=100);
#else
	////////////////////////////
	///构造函数
	///@param i2c i2c对象指针
	////////////////////////////
		mpu6050(I2C &i2c);
#endif
	
	///////////////////////////
	///析构函数
	///////////////////////////
		~mpu6050();



	//////////////////////////
	///IMU初始化函数,但不对iic进行初始化，在iic已经初始化了的情况下使用，否则会出错哦
	///@param wait if wait until the command in queue execute complete
	///@retval -1:检测到硬件 -2:检测到部分硬件 -3:正在检测 -0:未检测到硬件
	//////////////////////////
	virtual u8 Init(bool wait=false);//初始化IMU


#ifdef MPU6050_USE_TASKMANAGER
	/////////////////////////////
	///Set the max interval of update(Hz)
	///@param maxUpdateFrequency
	//////////////////////////
	void SetMaxUpdateFrequency(u16 maxUpdateFrequency);
	
	/////////////////////////////
	///Get the max interval of update(Hz)
	///@retval maxUpdateFrequency
	//////////////////////////
	u16 GetMaxUpdateFrequency();
#endif
	
	///将MPU650设置为bypass模式，可以将mpu6050的从机直接挂在与master连接的总线上
	///@param wait if wait until the command in queue execute complete
	///@retval true:set succed false:set register fail
	/////////////////////////
		bool BypassMode(bool wait=false);
	
	////////////////////////////////
	///获取加速度原始值
	///@retval 返回三轴的加速度值
	///////////////////////////////
	virtual	Vector3<int> GetAccRaw();//获取加速度原始值


	////////////////////////////////
	///获取角速度原始值
	///@retval 返回三轴的角速度值 
	///////////////////////////////
	virtual	Vector3<int> GetGyrRaw();//获取角速度原始值

		
	////////////////////////////////
	///获取加速度计的温度原始值
	///@retval 返回加速度计的温度值
	///////////////////////////////
		int GetTempRaw();//获取加速度计温度原始值
	
	////////////////////////////////
	///获取角速度值
	///@retval 返回三轴的角速度值 
	///////////////////////////////
	virtual	Vector3f GetGyr();//获取角速度原始值
		
	////////////////////////////////
	///获取加速度值
	///@retval 返回三轴的加速度值
	///////////////////////////////
	virtual	Vector3f GetAcc();//获取加速度原始值

	///////////////////////////
	///角速度校准值
	//////////////////////////
	virtual Vector3<int> GetGyrOffset();
	
	/////////////////////////////
	///角速度校准是否完毕
	//////////////////////////////
	virtual bool IsGyroCalibrated()
	{
		return mIsGyrCalibrated;
	}
	
	
	//////////////////////////////
	///从硬件更新加速度、角速度值到内存
	///@param wait if wait until the command in queue execute complete
	///@param acc The address of raw accelerator data save to 
	///@param gyro The address of raw gyro data save to
	///@return if wait set to true,MOD_READY:update succed MOD_ERROR:update fail  MOD_BUSY:Update interval is too short
	///        if wait set to false,MOD_ERROR:发送更新数据失败 MOD_READY:命令将会发送（具体的发送时间取决于队列中的排队的命令的数量）MOD_BUSY:Update interval is too short
	//////////////////////////////
	virtual	u8 Update(bool wait=false,Vector3<int> *acc=0, Vector3<int> *gyro=0);


	////////////////////////
	///开始角速度校准
	///@attention 校准时不要移动传感器
	/////////////////////////
	virtual void StartGyroCalibrate();
	
	////////////////////////
	///停止角速度校准
	///@attention 校准时不要移动传感器
	/////////////////////////
	virtual void StopGyroCalibrate();
	
	
	////////////////////
	///是否正在校准角速度
	///////////////////////
	virtual bool IsGyroCalibrating();
	
	///////////////////////////////////
	///检测mpu6050状态,状态来自于调用update函数之后
	///@retval 0:失去连接   1:正常状态     2:未初始化
	//////////////////////////////////
		u8 GetHealth();
		
		
	//////////////////////////////////////
	///获得两次数据采集的间隔
	//////////////////////////////////////
	virtual double GetUpdateInterval();
};


/* MPU6050 Register Address ------------------------------------------------------------*/
#define	SMPLRT_DIV					0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG							0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		 			0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	 			0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	 			0x3B
#define	ACCEL_XOUT_L 				0x3C
#define	ACCEL_YOUT_H				0x3D
#define	ACCEL_YOUT_L				0x3E
#define	ACCEL_ZOUT_H				0x3F
#define	ACCEL_ZOUT_L	 			0x40
#define	TEMP_OUT_H					0x41
#define	TEMP_OUT_L					0x42
#define	GYRO_XOUT_H		 			0x43
#define	GYRO_XOUT_L		 			0x44	
#define	GYRO_YOUT_H		 			0x45
#define	GYRO_YOUT_L		 			0x46
#define	GYRO_ZOUT_H		 			0x47
#define	GYRO_ZOUT_L					0x48
#define	PWR_MGMT_1		 			0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			 			0x75	//IIC地址寄存器(默认数值0x68，只读)

#define MPU6050_ADDRESS   MPU6050_DEFAULT_ADDRESS
#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
#define MPU6050_DEFAULT_ADDRESS     0xD0   //MPU6050_ADDRESS_AD0_LOW  ((MPU6050_ADDRESS_AD0_LOW<<1)&0xFE) or  ((MPU6050_ADDRESS_AD0_HIGH<<1)&0xFE)

#define I2C_MST_CTRL        0x24  
#define I2C_SLV0_ADDR       0x25  //指定从机的IIC地址
#define I2C_SLV0_REG        0x26 	//指定从机的寄存器地址 
#define I2C_SLV0_CTRL       0x27
#define I2C_SLV0_DO         0x63   //该寄存器的内容会写入到从机设备中
#define USER_CTRL           0x6A    //用户使能FIFO缓存区    I2C主机模式和主要I2C接口

#define INT_PIN_CFG         0x37  
#define EXT_SENS_DATA_00    0x49
#define EXT_SENS_DATA_01    0x4A
#define EXT_SENS_DATA_02    0x4B
#define EXT_SENS_DATA_03    0x4C
#define EXT_SENS_DATA_04    0x4D
#define EXT_SENS_DATA_05    0x4E
#define EXT_SENS_DATA_06    0x4F
#define EXT_SENS_DATA_07    0x50
#define EXT_SENS_DATA_08    0x51
#define EXT_SENS_DATA_09    0x52
#define EXT_SENS_DATA_10    0x53
#define EXT_SENS_DATA_11    0x54



#endif

