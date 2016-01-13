#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H
#endif
#include <vector>
#include "DOF.h"
#include "channel.h"

class Animation
{

public:

	float rangeMax;

	float rangeMin;

	std::vector<channel> channels;

	std::vector<DOF*> dofs;
	Animation(std::vector<DOF*> dofs);

	Animation();

	~Animation();

	bool Load(const char *file);

	Animation(const char*file, std::vector < DOF*> dof);

	void evaluate(float t);

	void preCompute();
	
};

