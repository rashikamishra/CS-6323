#include "joint.h"


Joint::Joint()
{
}

Joint::Joint(std::string bonename)
{
	name = bonename;
	dofx->setName(name);
	dofy->setName(name);
	dofz->setName(name);
	localMatrix->Identity();
	scale->Identity();
	translation->Identity();
	rotationX->Identity();
	rotationY->Identity();
	rotationZ->Identity();
	position = 0;
}

std::string Joint::getName()
{
	return name;
}

void Joint::setDOFLimitX(float max, float min)
{
	dofx->setLimit(max, min);
}

void Joint::setDOFLimitY(float max, float min)
{
	dofy->setLimit(max, min);
}

void Joint::setDOFLimitZ(float max, float min)
{
	dofz->setLimit(max, min);
}

void Joint::setDOFX(float value)
{
	dofx->setValue(value);
	this->pose.x = value;
}

void Joint::setDOFY(float value)
{
	dofy->setValue(value);
	this->pose.y = value;
}

void Joint::setDOFZ(float value)
{
	dofz->setValue(value);
	this->pose.z = value;
}

void Joint::setPose(Vector3 pose)
{
	setDOFX(pose.x);
	setDOFY(pose.y);
	setDOFZ(pose.z);
}


void Joint::setScale(Vector3 max, Vector3 min)
{
	maxBox = max;
	minBox = min;
	Vector3 scalar = max - min;
	scale->MakeScale(scalar);
}
void Joint::reset()
{
	setPose(pose);
	for (unsigned int i = 0; i < children.size(); i++)
	{
		Joint temp = children[i];
		temp.reset();
	}
}
void Joint::setTranslation(Vector3 move)
{
	offset = move;
	translation->MakeTranslate(move);
}

void Joint::calculate(Matrices m)
{
	localMatrix->Identity();
	localMatrix->Dot(*localMatrix, *translation);
	rotationZ->MakeRotateZ(dofz->value);
	localMatrix->Dot(*localMatrix, *rotationZ);
	rotationY->MakeRotateY(dofy->value);
	localMatrix->Dot(*localMatrix, *rotationY);
	rotationX->MakeRotateX(dofx->value);
	localMatrix->Dot(*localMatrix, *rotationX);
	worldMatrix->Dot(m, *localMatrix);
	for (unsigned int i = 0; i < children.size(); i++) 
	{
		Joint temp = children[i];
		temp.calculate(*worldMatrix);
	}
}

void Joint::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*worldMatrix);
	drawWireBox(minBox.x, minBox.y, minBox.z, maxBox.x, maxBox.y, maxBox.z);
	for (unsigned int i = 0; i < children.size(); i++) {
		Joint temp = children[i];
		temp.draw();
	}
}

void Joint::addChild(Joint c)
{
	children.push_back(c);
}

void Joint::getJoints(std::vector<Matrices*> &worldMatrixes, std::vector<DOF*> &joints)
{
	worldMatrixes.push_back(worldMatrix);
	joints.push_back(dofx);
	joints.push_back(dofy);
	joints.push_back(dofz);
	for (unsigned int i = 0; i < children.size(); i++) {
		Joint temp = children[i];
		temp.getJoints(worldMatrixes, joints);
	}
}

bool Joint::Load(Parser &reader) 
{
	reader.FindToken("{");
	Vector3 offset;
	Vector3 boxmin;
	Vector3 boxmax;
	float min;
	float max;
	Vector3 pose;
	while (1) 
	{
		char temp[256];
		reader.GetToken(temp);
		if (strcmp(temp, "offset") == 0) 
		{
			offset.x = reader.GetFloat();
			offset.y = reader.GetFloat();
			offset.z = reader.GetFloat();
			setTranslation(offset);
		}
		else if (strcmp(temp, "boxmin") == 0) 
		{
			boxmin.x = reader.GetFloat();
			boxmin.y = reader.GetFloat();
			boxmin.z = reader.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0) 
		{
			boxmax.x = reader.GetFloat();
			boxmax.y = reader.GetFloat();
			boxmax.z = reader.GetFloat();
			setScale(boxmax, boxmin);
		}
		else if (strcmp(temp, "rotxlimit") == 0) 
		{
			min = reader.GetFloat();
			max = reader.GetFloat();
			setDOFLimitX(max, min);
		}
		else if (strcmp(temp, "rotylimit") == 0) 
		{
			min = reader.GetFloat();
			max = reader.GetFloat();
			setDOFLimitY(max, min);
		}
		else if (strcmp(temp, "rotzlimit") == 0) 
		{
			min = reader.GetFloat();
			max = reader.GetFloat();
			setDOFLimitZ(max, min);
		}
		else if (strcmp(temp, "pose") == 0) 
		{
			pose.x = reader.GetFloat();
			pose.y = reader.GetFloat();
			pose.z = reader.GetFloat();
			setPose(pose);
		}
		else if (strcmp(temp, "balljoint") == 0) 
		{
			char name[256];
			reader.GetToken(name);
			Joint *jnt = new Joint(name);
			jnt->Load(reader);
			addChild(*jnt);
		}
		else if (strcmp(temp, "}") == 0) 
			return true;
		else 
			reader.SkipLine();
	}
}

Joint::~Joint()
{
}
