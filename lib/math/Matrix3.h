#ifndef _MATRIX3_H_
#define _MATRIX3_H_
#include "Quaternion.h"
#include "Vector3.h"

template <typename T>
class Matrix3
{
public:
	Vector3<T> a; //the 1st row
	Vector3<T> b; //the 2nd row
  Vector3<T> c; //the 3rd row
public:
	Matrix3<T>() { } //zero the vector elements
	Matrix3<T>(const Vector3<T> &a0, const Vector3<T> &b0, const Vector3<T> &c0) : a(a0), b(b0), c(c0) {}
	Matrix3<T>(const T ax, const T ay, const T az, const T bx, const T by, const T bz, const T cx, const T cy, const T cz) : a(ax,ay,az), b(bx,by,bz), c(cx,cy,cz) { }
	void operator() (const Vector3<T> &a0, const Vector3<T> &b0, const Vector3<T> &c0) {a = a0; b = b0; c = c0; }
	bool operator==(const Matrix3<T> &m) {  return (a==m.a && b==m.b && c==m.c);  }
	bool operator!= (const Matrix3<T> &m){  return (a!=m.a || b!=m.b || c!=m.c);  }
	Matrix3<T> operator-(void) const     {  return Matrix3<T>(-a,-b,-c);       }
  Matrix3<T> operator-(const Matrix3<T> &m) const   {  return Matrix3<T>(a-m.a, b-m.b, c-m.c); }
	Matrix3<T> operator+(const Matrix3<T> &m) const   {  return Matrix3<T>(a+m.a, b+m.b, c+m.c); }
	Matrix3<T> operator*(const T num) const           {  return Matrix3<T>(a*num, b*num, c*num); }
	Matrix3<T> operator/(const T num) const           {  return Matrix3<T>(a/num, b/num, c/num); }
	Matrix3<T> &operator+= (const Matrix3<T> &m)      {  return *this = *this + m;   }
	Matrix3<T> &operator-= (const Matrix3<T> &m)      {  return *this = *this - m;   }
	Matrix3<T> &operator*= (const T num)              {  return *this = *this * num; }
	Matrix3<T> &operator/= (const T num)              {  return *this = *this / num; }
	Vector3<T> operator *(const Vector3<T> &v) const	{  return Vector3<T>(a.x*v.x+a.y*v.y+a.z*v.z,	b.x*v.x+b.y*v.y+b.z*v.z, c.x*v.x+c.y*v.y+c.z*v.z);	}
	Matrix3<T> operator *(const Matrix3<T> &m) const
	{
		Matrix3<T> temp (Vector3<T>(a.x*m.a.x+a.y*m.b.x+a.z*m.c.x, a.x*m.a.y+a.y*m.b.y+a.z*m.c.y,	a.x*m.a.z+a.y*m.b.z+a.z*m.c.z),
										 Vector3<T>(b.x*m.a.x+b.y*m.b.x+b.z*m.c.x, b.x*m.a.y+b.y*m.b.y+b.z*m.c.y,	b.x*m.a.z+b.y*m.b.z+b.z*m.c.z),
										 Vector3<T>(c.x*m.a.x+c.y*m.b.x+c.z*m.c.x, c.x*m.a.y+c.y*m.b.y+c.z*m.c.y,	c.x*m.a.z+c.y*m.b.z+c.z*m.c.z));
		return temp;
	}
	void Zero(void) { a.x = a.y = a.z = 0;    b.x = b.y = b.z = 0;   c.x = c.y = c.z = 0; }
	void Identity(void) { a.x = b.y = c.z = 1;    a.y = a.z = 0;      b.x = b.z = 0;   c.x = c.y = 0;   }	
	bool IsNan(void)    { return a.IsNan() || b.IsNan() || c.IsNan();   }
	void Normalize(void)
	{
			float error = a * b;
			Vector3<T> t0 = a - (b * (0.5f * error));
			Vector3<T> t1 = b - (a * (0.5f * error));
			Vector3<T> t2 = t0 % t1;
			a = t0 * (1.0f / t0.Length());
			b = t1 * (1.0f / t1.Length());
			c = t2 * (1.0f / t2.Length());
	}
	void Rotate(const Vector3<T> &g)
	{
		Matrix3<T> temp_matrix;
		temp_matrix.a.x = a.y * g.z - a.z * g.y;
		temp_matrix.a.y = a.z * g.x - a.x * g.z;
		temp_matrix.a.z = a.x * g.y - a.y * g.x;
		temp_matrix.b.x = b.y * g.z - b.z * g.y;
		temp_matrix.b.y = b.z * g.x - b.x * g.z;
		temp_matrix.b.z = b.x * g.y - b.y * g.x;
		temp_matrix.c.x = c.y * g.z - c.z * g.y;
		temp_matrix.c.y = c.z * g.x - c.x * g.z;
		temp_matrix.c.z = c.x * g.y - c.y * g.x;
		(*this) += temp_matrix;
	}
	Matrix3<T> Transpose(void) const{ return Matrix3<T>(Vector3<T>(a.x,b.x,c.x), Vector3<T>(a.y,b.y,c.y), Vector3<T>(a.z,b.z,c.z)); }
	void ToEuler(float *roll=0, float *pitch=0, float *yaw=0) const
	{
		if (pitch != 0) {	*pitch = -asinf(c.x);		}
		if (roll  != 0) {	*roll  = atan2f(c.y, c.z);	}
		if (yaw   != 0) {	*yaw   = atan2f(b.x, a.x);	}
	}
};


#endif


