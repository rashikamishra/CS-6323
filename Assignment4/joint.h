#ifndef JOINT_H
#define JOINT_H
#endif

#include <string>
#include <vector>
#include <iostream>

#include "DOF.h"

#ifndef MATRICES_H
#include "Matrices.h"
#endif

#ifndef PARSER_H
#include "Parser.h"
#endif

#ifndef UTILS_H
#include "Utils.h"
#endif

class Joint
{
private: 
	std::string name;

	DOF *dofx = new DOF();
	DOF *dofy = new DOF();
	DOF *dofz = new DOF();

	Matrices *localMatrix = new Matrices();
	Matrices *scale = new Matrices();
	Matrices *translation = new Matrices();
	Matrices *rotationX = new Matrices();
	Matrices *rotationY = new Matrices();
	Matrices *rotationZ = new Matrices;
	Matrices *worldMatrix = new Matrices();

	Vector3 offset;
	Vector3 minBox;
	Vector3 maxBox;

	Vector3 pose;

	std::vector<Joint> children;

	int position;

public:
	Joint();

	Joint(std::string bonename);

	std::string getName();

	void setDOFLimitX(float max, float min);

	void setDOFLimitY(float max, float min);

	void setDOFLimitZ(float max, float min);

	void setDOFX(float value);

	void setDOFY(float value);

	void setDOFZ(float value);

	void setPose(Vector3 pose);

	void setScale(Vector3 max, Vector3 min);

	void reset();

	void setTranslation(Vector3 move);

	void calculate(Matrices m);

	void draw();

	void addChild(Joint c);

	bool Load(Parser &reader);

	void getJoints(std::vector<Matrices*> &worldMatrixes, std::vector<DOF*> &joints);

	~Joint();
};
