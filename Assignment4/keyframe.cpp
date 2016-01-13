#include "keyframe.h"


keyframe::keyframe()
{
}


keyframe::~keyframe()
{
}


void keyframe::setLinearTangentIn(keyframe prev) 
{
	tangentInValue = ((keyframeValue - prev.keyframeValue) / (time - prev.time));
}

void keyframe::setLinearTangentOut(keyframe next) 
{
	tangentOutValue = ((next.keyframeValue - keyframeValue) / (next.time - time));
}

void keyframe::setSmoothTangent(keyframe prev, keyframe next) 
{
	tangentInValue = ((next.keyframeValue - prev.keyframeValue) / (next.time - prev.time));
	tangentOutValue = tangentInValue;
}

void keyframe::calculateCoeficients(keyframe next) 
{
	float newTangent = (next.time - time) * tangentOutValue;
	float newNextTangent = (next.time - time) * next.tangentOutValue;
	coeffA = 2 * keyframeValue + -2 * next.keyframeValue + newTangent + newNextTangent;
	coeffB = -3 * keyframeValue + 3 * next.keyframeValue + -2 * newTangent + -1 * newNextTangent;
	coeffC = newTangent;
	coeffD = keyframeValue;
}

float keyframe::evaluate(float evalTime, keyframe next) 
{
	float newTime = (evalTime - time) / (next.time - time);
	return (coeffD + newTime*(coeffC + newTime *(coeffB + newTime *(coeffA))));
}