
#include <vector>
#include "Matrices.h"
#include "Vectors.h"
#include <string>

struct Joint
{
	Matrix4 transform; 

	Joint* parentJoint;

	std::vector< Joint* > children; 
	Matrix4 xRotation;
	Matrix4 yRotation;
	Matrix4 zRotation;
	std::string Name;

	Vector3 offset;

	Vector3 boxMin;

	Vector3 boxMax;

	Vector3 pose;

	float rotXMinLimit = -10000;
	float rotXMaxLimit = 10000;
	float rotYMinLimit = -10000;
	float rotYMaxLimit = 10000;
	float rotZMinLimit = -10000;
	float rotZMaxLimit = 10000;


};