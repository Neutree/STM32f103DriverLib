#ifndef __AHRS_DCM_H__
#define __AHRS_DCM_H__

#include "Matrix3.h"
class AHRS_DCM
{
public:
	AHRS_DCM()
	{
		mDCMVector.x = 0;
		mDCMVector.y = 0;
		mDCMVector.z = 9.7935f;
	}
	Vector3f GetAngle_InertialSensor(Vector3<int> acc,Vector3f gyro,float updateInterval)
	{
//		Vector3f acc ;
//		acc.x = acc_.x;
//		acc.y = acc_.y;
//		acc.z = acc_.z;
//		com<<acc.x<<"\t"<<acc.y<<"\t"<<acc.z<<"\t"<<gyro.x<<"\t"<<gyro.y<<"\t"<<gyro.z<<"\n";
		Vector3f delta = gyro*updateInterval;
		delta.x = -delta.x;
	
		Vector3f old = mDCMVector;
		
		mDCMVector.x += delta.x*old.z - delta.z*old.y;
		mDCMVector.y += delta.y*old.z + delta.z*old.x;
		mDCMVector.z -= delta.x*old.x + delta.y*old.y;
		
		//float acclen = _acc.x*_acc.x+_acc.y*_acc.y+_acc.z*_acc.z;
		//if(acclen>0.36f && acclen<1.96f)
		{
			float accFilter = 0.002;
			mDCMVector.x = mDCMVector.x*(1-accFilter) + acc.y*accFilter;
			mDCMVector.y = mDCMVector.y*(1-accFilter) + acc.x*accFilter;
			mDCMVector.z = mDCMVector.z*(1-accFilter) - acc.z*accFilter;
		}
		
		mAngle.x = atan2f(mDCMVector.x, sqrtf(mDCMVector.y*mDCMVector.y+mDCMVector.z*mDCMVector.z))*57.29f;
		mAngle.y = atan2f(mDCMVector.y,mDCMVector.z)*57.29f;
		return mAngle;
	}
	Vector3f GetAngle_InertialSensor_Mag()
	{
		return mDCMVector;
	}
private:
	Matrix3<float> mDCMMatrix;
	Vector3f mDCMVector;

	Vector3f mAngle;     //算出来的角度
};




#endif
