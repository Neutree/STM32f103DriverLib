#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include "math.h"

template <typename T>
class Vector3
{
public:
	T	x,y,z;
public:
  //defualt constructor
	Vector3()	{	x = 0;  y = 0; z = 0;	}
	// setting ctor
	Vector3(T x0, T y0, T z0) : x(x0), y(y0), z(z0)	{}
  //"()" overload 
  void operator()(T x0, T y0, T z0){x= x0; y= y0; z= z0;}
  //"==" overload
	bool operator==(const Vector3<T> &v){return (x==v.x && y==v.y && z==v.z);	}
  //"!=" overload
  bool operator!=(const Vector3<T> &v){return (x!=v.x || y!=v.y || z!=v.z); }
 // "-" negation overload
	Vector3<T> operator-(void) const {	return Vector3<T>(-x,-y,-z);}
  //"+" addition overload
	Vector3<T> operator+(const Vector3<T> &v) const	{	return Vector3<T>(x+v.x, y+v.y, z+v.z);	}
  //"-" subtraction overload
  Vector3<T> operator-(const Vector3<T> &v) const { return Vector3<T>(x-v.x, y-v.y, z-v.z); }
  //"*" multiply overload
  Vector3<T> operator*(const T n)const { return Vector3<T>(x*n, y*n, z*n); }
  //"/" divsion overload
	Vector3<T> operator/(const T n)const { return Vector3<T>(x/n, y/n, z/n); }
  //"=" 
	Vector3<T> &operator=(const Vector3<T> &v){x=v.x; y=v.y; z=v.z; return *this;}
  //"+=" overload
  Vector3<T> &operator+=(const Vector3<T> &v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
	//"-=" overload
	Vector3<T> &operator-=(const Vector3<T> &v)	{	x-=v.x; y-=v.y; z-=v.z;	return *this;	}
  //"*=" overload
	Vector3<T> &operator*=(const T n)	{		x*=n; y*=n; z*=n;		return *this;	}
  // uniform scaling
	Vector3<T> &operator/=(const T n)	{		x/=n; y/=n; z/=n;		return *this; }
  //dot product
  T operator*(const Vector3<T> &v) const { return x*v.x + y*v.y + z*v.z;  }
  //cross product
  Vector3<T> operator %(const Vector3<T> &v)const { return Vector3<T>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);}
  // gets the length of this vector squared
  T LengthSquared() const  {   return (T)(*this * *this); }
  // gets the length of this vector
  float Length(void) const  {   return (T)sqrt(*this * *this); }
  // normalizes this vector
  void Normalize() {  *this/=Length(); }
  // zero the vector
  void Zero()   {  x = y = z = 0.0;  }
  // returns the normalized version of this vector
  Vector3<T>  Normalized() const  {   return *this/Length();   }
	// check if any elements are NAN
	bool IsNan(void) { return isnan(x) || isnan(y) || isnan(z); }
	// check if any elements are infinity
	bool IsInf(void) { return isinf(x) || isinf(y) || isinf(z); }

};
typedef Vector3<float> Vector3f;
#endif // VECTOR3_H
