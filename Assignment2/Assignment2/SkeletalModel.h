
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include "Joint.h"
#ifndef UTILS_H
#include "Utils.h"
#endif

class SkeletalModel
{
public:
	void load(const char *skeletonFile);

	void loadSkeleton( const char* filename );

	void drawSkeleton(Utils *utils);

	void ComputeTransforms();


private:

	Joint* m_rootJoint;

	std::vector< Joint* > m_joints;

	Utils *utils;

};

