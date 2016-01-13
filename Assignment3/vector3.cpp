
#include "vector3.h"

Vector3::Vector3() : 
x(0.0f), y(0.0f), z(0.0f)	
{
}
Vector3::Vector3(float x0, float y0, float z0) :
x(x0), y(y0), z(z0)							
{
}

void Vector3::Set(float x0, float y0, float z0)			
{
	x = x0; 
	y = y0;
	z = z0;
}

void Vector3::Zero()
{
	x = y = z = 0.0f;
}

Vector3 Vector3::operator+ (const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

const Vector3 & Vector3::operator+= (const Vector3& v)
{
	x += v.x; y += v.y; z += v.z; return *this;
}

Vector3 Vector3::operator- (const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

const Vector3 & Vector3::operator-= (const Vector3& v)
{
	x -= v.x; 
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3 Vector3::operator* (float a) const 
{
	return Vector3(x * a, y * a, z * a); 
}

Vector3 Vector3::operator* (const Vector3& v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

const Vector3 & Vector3::operator*= (float a) 
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

const Vector3 & Vector3::operator*= (const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

Vector3 Vector3::operator- () const
{ 
	return Vector3(-x, -y, -z);
}

const Vector3 & Vector3::Negate()
{
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

Vector3 Vector3::operator/ (float a) const
{
	float inv = 1.0f / a;
	return Vector3(x * inv, y * inv, z * inv);
}

Vector3 Vector3::operator/ (const Vector3 & v) const
{
	return Vector3(x / v.x, y / v.y, z / v.z);
}

const Vector3 & Vector3::operator/= (float a)
{
	float inv = 1.0f / a;
	x *= inv;
	y *= inv;
	z *= inv;
	return *this;
}

const Vector3 & Vector3::operator/= (const Vector3 & v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

float Vector3::Mag2() const
{ 
	return x * x + y * y + z * z;
}

float Vector3::Mag() const 
{
	return sqrtf(Mag2());
}

const Vector3 & Vector3::Normalize()
{ 
	return (*this /= Mag());
}

float Vector3::Dot(const Vector3 & a) const
{ 
	return x*a.x + y*a.y + z*a.z;
}

void Vector3::Cross(const Vector3 & a, const Vector3 & b)
{
	x = a.y*b.z - a.z*b.y;
	y = a.z*b.x - a.x*b.z;
	z = a.x*b.y - a.y*b.x;
}

void Vector3::Lerp(float t, const Vector3 a, const Vector3 b)
{ 
	*this = a; 
	*this *= 1.0f - t; 
	x += b.x*t;
	y += b.y*t;
	z += b.z*t;
}

float Vector3::Dist2(const Vector3 a) const
{
	return (x - a.x)*(x - a.x) + (y - a.y)*(y - a.y) + (z - a.z)*(z - a.z); 
}

float &Vector3::operator[](int i)
{
	return(((float*)this)[i]);
}


Vector3 Vector3::XAXIS(1.0f, 0.0f, 0.0f);
Vector3 Vector3::YAXIS(0.0f, 1.0f, 0.0f);
Vector3 Vector3::ZAXIS(0.0f, 0.0f, 1.0f);
Vector3 Vector3::ORIGIN(0.0f, 0.0f, 0.0f);