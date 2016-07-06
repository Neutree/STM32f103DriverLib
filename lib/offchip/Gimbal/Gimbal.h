#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "PIDController.h"
#include "BLDCMotor.h"
#include "InertialSensor.h"
#include "AHRS_DCM.h"
#include "ADC.h"

#include "Configuration.h"

class Gimbal
{
private:
	InertialSensor& mIns;	
	
	BLDCMotor& mMotorRoll;
	BLDCMotor& mMotorPitch;
	BLDCMotor& mMotorYaw;
	ADC& mADC;
	

	AHRS_DCM mAhrs_dcm;
	Vector3f mAngle;

	bool mIsCalibrating;

public:
	Gimbal(InertialSensor& ins,BLDCMotor& motorRoll,BLDCMotor& motorPitch,BLDCMotor& motorYaw,ADC& adc);
	bool Init();
	bool UpdateIMU();
	bool UpdateMotor();
	float UpdateVoltage(uint8_t channelNumber,float resister_a,float resister_b,float fullRange);
	bool IsCalibrated();
	bool IsCalibrating();
	
	PIDController mPIDRoll,mPIDPitch,mPIDYaw;
};

#endif

