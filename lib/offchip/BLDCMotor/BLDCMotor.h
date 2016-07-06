#ifndef __BLDCMOTOR_H
#define __BLDCMOTOR_H
#include "PWM.h"
#include "math.h"

class BLDCMotor
{
	private:
		static bool bInitSPWM;
		static float SPWM[256];
		PWM *_pwm_a;
		PWM *_pwm_b;
		PWM *_pwm_c;
		u8 _ch_a;
		u8 _ch_b;
		u8 _ch_c;
		float _maxPower;
		bool _armed;
	private:
		static void InitSPWM();
	public:
		BLDCMotor(PWM *pwma, u8 cha, PWM *pwmb, u8 chb, PWM *pwmc, u8 chc, float power);
		void Initialize(PWM *pwma, u8 cha, PWM *pwmb, u8 chb, PWM *pwmc, u8 chc, float power);
		void SetPosition(int pos);
		void Enable();
		void Disable();
};

#endif

