#include "channel.h"


channel::channel()
{
	keyframes = std::vector<keyframe>();
}


channel::~channel()
{
}

void channel::preLoad()
{
	for (int i = 0; i < keyframes.size(); i++) 
	{
		if (keyframes[i].tangentInMode.compare("flat") == 0) 
		{
			keyframes[i].tangentInValue = 0;
		}
		else if (keyframes[i].tangentInMode.compare("linear") == 0) 
		{
			if (i == 0)
				keyframes[i].tangentInValue = 0;
			else
				keyframes[i].setLinearTangentIn(keyframes[i - 1]);
		}
		else if (keyframes[i].tangentInMode.compare("smooth") == 0)
		{
			if (i == 0 && i == keyframes.size() - 1) 
			{
				keyframes[i].tangentInValue = 0;
				keyframes[i].tangentOutValue = 0;
			}
			else if (i == 0) 
			{
				keyframes[i].tangentInValue = 0;
				keyframes[i].setLinearTangentOut(keyframes[i + 1]);
			}
			else if (i == keyframes.size() - 1) {
				keyframes[i].setLinearTangentIn(keyframes[i - 1]);
				keyframes[i].tangentOutValue = 0;
			}
			else
				keyframes[i].setSmoothTangent(keyframes[i - 1], keyframes[i + 1]);
		}
		if (keyframes[i].tangentOutMode.compare("flat") == 0) 
		{
			keyframes[i].tangentOutValue = 0;
		}
		else if (keyframes[i].tangentOutMode.compare("linear") == 0) 
		{
			if (i == keyframes.size() - 1)
				keyframes[i].tangentOutValue = 0;
			else
				keyframes[i].setLinearTangentOut(keyframes[i + 1]);
		}
	}
	for (int i = 0; i < keyframes.size() - 1; i++)
	{
		keyframes[i].calculateCoeficients(keyframes[i + 1]);
	}
	start = keyframes[0];
	end = keyframes[keyframes.size() - 1];
	length = keyframes[keyframes.size() - 1].time - start.time;
	
}
bool channel::Load(Parser &reader) 
{
	reader.FindToken("{");
	char temp[256];
	reader.GetToken(temp);
	if (strcmp(temp, "extrapolate") == 0) 
	{
		reader.GetToken(temp);
		extrapolateInMode = temp;
		reader.GetToken(temp);
		extrapolateOutMode = temp;
	}
	reader.GetToken(temp);
	if (strcmp(temp, "keys") == 0) 
	{
		int index = reader.GetInt();
		reader.FindToken("{");
		for (int i = 0; i < index; i++) 
		{
			keyframe *key = new keyframe();
			key->time = (reader.GetFloat());
			key->keyframeValue = (reader.GetFloat());
			reader.GetToken(temp);
			if (isdigit(temp[0])) 
			{
				key->tangentInValue = atof(temp);
				key->tangentOutMode = "constant value";
			}
			else
				key->tangentInMode = temp;
			reader.GetToken(temp);
			if (isdigit(temp[0])) 
			{
				key->tangentOutValue = atof(temp);
				key->tangentOutMode = "constant value";
			}
			else
				key->tangentOutMode = temp;
			keyframes.push_back(*key);
		}
		reader.FindToken("}");
		reader.FindToken("}");
	}
	return true;
}
float channel::evaluate(float t) 
{
	if (t < start.time) {
		if (extrapolateInMode.compare("constant") == 0) 
		{
			return start.keyframeValue;
		}
		else if (extrapolateInMode.compare("linear") == 0) 
		{
			float offset = start.keyframeValue - (start.time * start.tangentOutValue);
			return t * start.tangentOutValue + offset;
		}
		else if (extrapolateInMode.compare("cycle") == 0) 
		{

			float newT = fmod(end.time - t, length);
			newT = end.time - newT;
			return evaluate(newT);
		}
		else if (extrapolateInMode.compare("cycle_offset") == 0) 
		{
			float newT = fmod(end.time - t, length);
			newT = end.time - newT;
			int offset = (end.time - t) / length;
			return evaluate(newT) - offset * (end.keyframeValue - start.keyframeValue);
		}
		else if (extrapolateInMode.compare("bounce") == 0)
		{
			float newT = fmod(end.time - t, length);
			newT = end.time - newT;
			int drctn = (t - start.time) / length;
			if (drctn % 2 == 0)
				return evaluate(end.time - newT);
			else
				return evaluate(newT);
		}
	}
	else if (t > end.time) 
	{
		if (extrapolateOutMode.compare("constant") == 0)
		{
			return end.keyframeValue;
		}
		else if (extrapolateInMode.compare("linear") == 0) 
		{
			float offset = end.keyframeValue - (end.time * end.tangentInValue);
			return t * end.tangentInValue + offset;
		}
		else if (extrapolateInMode.compare("cycle") == 0) 
		{
			float newT = fmod(t - start.time, length) + start.time;
			return evaluate(newT);
		}
		else if (extrapolateInMode.compare("cycle_offset") == 0)
		{
			float newT = fmod(t - start.time, length);
			newT = newT + start.time;
			int offset = (t - start.time) / length;
			return evaluate(newT) + offset * (end.keyframeValue - start.keyframeValue);
		}
		else if (extrapolateInMode.compare("bounce") == 0) 
		{
			float newT = fmod(t - start.time, length);
			newT = newT	+ start.time;
			int drctn = (t - start.time) / length;
			if (drctn % 2 == 1)
				return evaluate(end.time - newT);
			else
				return evaluate(newT);
		}
	}
	else
	{
		for (int i = 0; i < keyframes.size(); i++) 
		{
			if (t < keyframes[i].time) {
				return keyframes[i - 1].evaluate(t, keyframes[i]);
			}
			else if (t == keyframes[i].time)
				return keyframes[i].keyframeValue;
		}
	}
}