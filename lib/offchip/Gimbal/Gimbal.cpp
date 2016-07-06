#include "Gimbal.h"


Gimbal::Gimbal(InertialSensor& ins,BLDCMotor& motorRoll,BLDCMotor& motorPitch,BLDCMotor& motorYaw,ADC& adc)
:mIns(ins),mMotorRoll(motorRoll),mMotorPitch(motorPitch),mMotorYaw(motorYaw),mADC(adc),mIsCalibrating(false)
{
	mPIDRoll(5,0.2,0.6);
	mPIDPitch(0,0,0);
	mPIDYaw(0,0,0);
}
bool Gimbal::Init()
{
	float time = TaskManager::Time();
	mMotorRoll.Disable();
	mMotorPitch.Disable();
	mMotorYaw.Disable();
	mIns.Init();
	while(TaskManager::Time()-time<1.5)
	{}
	mIns.StartGyroCalibrate();//启动校准
	mIsCalibrating = true;
	LOG("calibrating ... don't move!!!\n");
	return true;
}
bool Gimbal::UpdateIMU()
{
	if(MOD_ERROR== mIns.Update())
	{
		LOG("mpu6050 error\n\n\n");
		return false;
	}
	if(mIsCalibrating&&!mIns.IsGyroCalibrating())//角速度校准结束
	{
		mIsCalibrating = false;
		LOG("\ncalibrate complete\n");
		mMotorRoll.Enable();
		mMotorPitch.Enable();
		mMotorYaw.Enable();
	}
	if(mIns.IsGyroCalibrated())//角速度已经校准了
	{
		mAngle = mAhrs_dcm.GetAngle_InertialSensor(mIns.GetAccRaw(),mIns.GetGyr(),mIns.GetUpdateInterval());
	}
	return true;
}
bool Gimbal::UpdateMotor()
{
	int v = mPIDRoll.Controll(0,mAngle.x);
	mMotorRoll.SetPosition(v);
	LOG(v);
	LOG("\n");
	mMotorPitch.SetPosition(mPIDPitch.Controll(0,mAngle.y));
	mMotorYaw.SetPosition(0);
	return true;
}
float Gimbal::UpdateVoltage(uint8_t channelNumber,float resister_a,float resister_b,float fullRange)
{
	return mADC.Voltage_I(channelNumber,resister_a,resister_b,fullRange);
}
bool Gimbal::IsCalibrated()
{
	return mIns.IsGyroCalibrated();
}
bool Gimbal::IsCalibrating()
{
	return mIsCalibrating;
}

