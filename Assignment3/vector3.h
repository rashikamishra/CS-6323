#ifndef VECTOR3_H
#define VECTOR3_H
#endif

#ifndef UTILS_H
#include "Utils.h"
#endif

class Vector3 
{
	public:
		Vector3();

		Vector3(float x0, float y0, float z0);

		void Set (float x0, float y0, float z0);

		void Zero ();

		Vector3 operator+ (const Vector3& v) const;
	
		const Vector3 & operator+= (const Vector3& v);

		Vector3 operator- (const Vector3& v) const;
	
		const Vector3 & operator-= (const Vector3& v);

		Vector3 operator* (float a) const;

		Vector3 operator* (const Vector3& v) const;
	
		const Vector3 & operator*= (float a);
	
		const Vector3 & operator*= (const Vector3& v);
	
		Vector3 operator- () const;

		const Vector3 & Negate();

		Vector3 operator/ (float a) const;
	
		Vector3 operator/ (const Vector3 & v) const;
	
		const Vector3 & operator/= (float a);
	
		const Vector3 & operator/= (const Vector3 & v);

		float Mag2 () const;

		float Mag () const ;

		const Vector3 & Normalize();

		float Dot(const Vector3 & a) const;

		void Cross(const Vector3 & a,const Vector3 & b);

		void Lerp(float t,const Vector3 a,const Vector3 b);
																	
		float Dist2(const Vector3 a) const;
															
		float &operator[](int i);

		static Vector3 XAXIS,YAXIS,ZAXIS;

		static Vector3 ORIGIN;

	public:
		float x, y, z;
};

inline Vector3 operator* (float s, const Vector3& v)
{
    return Vector3 (v.x * s, v.y * s, v.z * s);
}

