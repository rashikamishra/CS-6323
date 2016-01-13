//Code reused from songoho matrices

#ifndef MATRICES_H
#define MATRICES_H
#endif

#ifndef VECTOR3_H
#include "vector3.h"
#endif

class Matrices 
{
	public:
		Matrices()										
		{ 
			Identity();
			pad0 = pad1 = pad2 = 0.0f; 
			pad3 = 1.0f; 
		}

		Matrices(float ax, float bx, float cx, float dx,
			 float ay,float by,float cy,float dy,
			 float az,float bz,float cz,float dz);

		void Identity();

		void Dot(const Matrices &n, const Matrices &m);	

		void Transform(const Vector3 &in,Vector3 &out) const;

		void Transform3x3(const Vector3 &in,Vector3 &out) const;

		void MakeRotateX(float t);

		void MakeRotateY(float t);

		void MakeRotateZ(float t);

		void MakeRotateUnitAxis(const Vector3 &v,float t);	

		void MakeScale(float x,float y,float z);

		void MakeScale(const Vector3 &v)		
		{
			MakeScale(v.x,v.y,v.z);
		}

		void MakeScale(float s)							
		{
			MakeScale(s,s,s);
		}

		void MakeTranslate(float x,float y,float z)		
		{
			Identity(); 
			d.Set(x,y,z);
		}

		void MakeTranslate(const Vector3 &v)		
		{
			Identity(); 
			d=v;
		}

		enum {EULER_XYZ,EULER_XZY,EULER_YXZ,EULER_YZX,EULER_ZXY,EULER_ZYX};

		void FromEulers(float ang1, float ang2, float ang3, int order);

		void ToEulers(float* ang1, float* ang2, float* ang3, int order);

		bool Inverse();						

		void FastInverse();				

		void Transpose();							

		void LookAt(const Vector3 &from,const Vector3 &to);

		void PolarView(float dist,float azm,float inc,float twst=0);

		float Determinant3x3() const;

		Vector3 &operator[](int i)						
		{ 
			return *((Vector3*)&(((float*)this)[i * 4])); 
		}

		operator float*()								
		{
			return (float*)this;
		}

		static Matrices IDENTITY;

	public:
		Vector3 a; float pad0;

		Vector3 b; float pad1;

		Vector3 c; float pad2;

		Vector3 d; float pad3;
};