#ifndef KEYFRAME_H
#define KEYFRAME_H
#endif
#include <string>
#include <iostream>
class keyframe
{
private:

	float coeffA, coeffB, coeffC, coeffD;
public:
	float time;

	float keyframeValue;

	float tangentInValue;

	float tangentOutValue;

	std::string tangentInMode;

	std::string tangentOutMode;

	keyframe();

	~keyframe();

	void setLinearTangentIn(keyframe prev) ;

	void setLinearTangentOut(keyframe next) ;

	void setSmoothTangent(keyframe prev, keyframe next) ;

	void calculateCoeficients(keyframe next) ;

	float evaluate(float evalTime, keyframe next);
};


