#include "DOF.h"


DOF::DOF()
{
	max = 100000;
	min = -100000;
	value = 0;
}

DOF::DOF(std::string name)
{
	this->name = name;
}

std::string DOF::getName()
{
	return name;
}

void DOF::setName(std::string name)
{
	this->name = name;
}

void DOF::setLimit(float maxVal, float minVal)
{
	max = maxVal;
	min = minVal;
}

void DOF::setValue(float val) 
{
	if (max > val)
		if (val > min)
			value = val;
	if (max <= val)
		value = max;
	if (val <= min)
		value = min;
}

DOF::~DOF()
{
}
