#ifndef __PIDCONTROLLER_H
#define __PIDCONTROLLER_H

class PIDController
{
private:
	float mKp;
	float mKi;
	float mKd;

	float _Perr;
	float _Ierr;
	float _Derr;

public:
	PIDController(float kp=0, float ki=0, float kd=0)
	{
		mKp = kp;
		mKi = ki;
		mKd = kd;
	}
	void operator()(float kp, float ki, float kd)
	{
		mKp = kp;  
		mKi = ki;  
		mKd = kd; 
	}
	float Controll(float target, float now)
	{
		float err = target - now;
		
		float detaI = err*mKi*2;
		
		if(detaI>10)  detaI = 10; 
		if(detaI<-10) detaI = -10;
			
		_Ierr += detaI;
		_Derr =  (err -_Perr)*500;
		_Perr = err;
		return mKp*_Perr + _Ierr + mKd*_Derr;
	}
	void AddKp(float value)
	{
		mKp+=value;
	}
	void AddKi(float value)
	{
		mKi+=value;
	}
	void AddKd(float value)
	{
		mKd+=value;
	}
	float GetKp()
	{
		return mKp;
	}
	float GetKi()
	{
		return mKi;
	}
	float GetKd()
	{
		return mKd;
	}
};

#endif

