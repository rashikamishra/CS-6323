#ifndef UTILS_H
#include "Utils.h"
#endif

class Camera {
public:
	Camera();

	void Update();
	void Reset();
	void Draw();

	// Access functions
	void SetAspect(float a)		{Aspect=a;}
	void SetDistance(float d)	{Distance=d;}
	void SetAzimuth(float a)	{Azimuth=a;}
	void SetIncline(float i)	{Incline=i;}

	float GetDistance()			{return Distance;}
	float GetAzimuth()			{return Azimuth;}
	float GetIncline()			{return Incline;}

private:
	float FOV;		
	float Aspect;	
	float NearClip;	
	float FarClip;	

	
	float Distance;	
	float Azimuth;	
	float Incline;	
};

