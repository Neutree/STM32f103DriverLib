#ifndef _QUATERNION_H_
#define _QUATERNION_H_

#include "math.h"

class Quaternion
{
public:
	float q1, q2, q3, q4;    
public:
	// constructor creates a quaternion equivalent to roll=0, pitch=0, yaw=0
  Quaternion() {  q1 = 1; q2 = q3 = q4 = 0;  }
  // setting constructor
  Quaternion(float _q1, float _q2, float _q3, float _q4):q1(_q1), q2(_q2), q3(_q3), q4(_q4) {}
  // function call operator
  void operator()(float _q1, float _q2, float _q3, float _q4) {q1 = _q1; q2 = _q2; q3 = _q3; q4 = _q4;  }
  // check if any elements are NAN
  bool IsNan(void) { return isnan(q1) || isnan(q2) || isnan(q3) || isnan(q4); }
  // create eulers from a quaternion
  void ToEuler(float &roll, float &pitch, float &yaw)
	{
    roll = atan2(2.0*(q1*q2 + q3*q4), 1 - 2.0*(q2*q2 + q3*q3));
    pitch = asin(2.0*(q1*q3 - q4*q2));
    yaw = atan2(2.0*(q1*q4 + q2*q3),  1 - 2.0*(q3*q3 + q4*q4));
	}
};
#endif // QUATERNION_H
