#ifndef _SENSOR_H_
#define _SENSOR_H_

#include "TaskManager.h"

class Sensor
{
	protected:
		bool   mHealthy;
		bool   mIsUpdated;                        //I2C task complete falg
		double mUpdatedTime;                      //I2C task completed time   (second)
		double  mInterval;                        //time interval since last update (second)
	public:
		Sensor()
		{
			mHealthy = false;
			mIsUpdated = true;
			mUpdatedTime = 0;
			mInterval = 0;
		}
		void Updated()
		{
			mIsUpdated = true;
			double curTime = tskmgr.Time();
			mInterval = curTime - mUpdatedTime;
			mUpdatedTime = curTime;
		}
		float Interval() {	return mInterval; }  //return interval between two update
		bool IsHealthy() {  return mHealthy;  }  //return healthy state of device
};


#endif






