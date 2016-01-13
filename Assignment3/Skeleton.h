#ifndef SKELETON_H
#define SKELETON_H
#endif

#include <vector>

#include "joint.h"
#include "DOF.h"

#ifndef PARSER_H
#include "Parser.h"
#endif

#ifndef MATRICES_H
#include "Matrices.h"
#endif

class Skeleton
{
private:
	Joint *Root;
public:
	Skeleton();

	DOF* x;
	DOF* y;
	DOF* z;

	Matrices translation;

	std::vector<Matrices*> worldMatrixes;

	std::vector<DOF*> joints;

	Skeleton(const char *file);

	bool Load(const char *file);

	void calculate(Matrices m);

	void draw();

	void reset();

	void getJoints();

	~Skeleton(); 
};

