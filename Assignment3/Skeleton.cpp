#include "Skeleton.h"

Skeleton::Skeleton()
{

}

Skeleton::Skeleton(const char *file)
{
	Load(file);
	Matrices identity;
	x = new DOF();
	y = new DOF();
	z = new DOF();
	x->setName("Root's Translation");
	y->setName("Root's Translation");
	z->setName("Root's Translation");
	joints.push_back(x);
	joints.push_back(y);
	joints.push_back(z);
	calculate(identity.IDENTITY);
	getJoints();
}

Skeleton::~Skeleton()
{
}

bool Skeleton::Load(const char *file) 
{
	Parser reader;
	reader.Open(file);
	reader.FindToken("balljoint");
	Root = new Joint("Root");
	Root->Load(reader);
	reader.Close();
	return true;
}

void Skeleton::calculate(Matrices m)
{
	translation.MakeTranslate(Vector3(x->value, y->value, z->value));
	m.Dot(translation, m); 
	Root->calculate(m);
}

void Skeleton::draw() 
{ 
	Root->draw(); 
}

void Skeleton::reset() 
{ 
	Root->reset();
	x->setValue(0); 
	y->setValue(0);
	z->setValue(0); 
}

void Skeleton::getJoints()
{ 
	Root->getJoints(worldMatrixes, joints);
}