#include "Animation.h"


Animation::Animation()
{
}

Animation::Animation(std::vector<DOF*> d)
{
	dofs = d;
}

Animation::Animation(const char *file, std::vector<DOF*> d) 
{
	dofs = d;
	Load(file);
	preCompute();
}

void Animation::evaluate(float t) 
{
	for (int i = 0; i < channels.size(); i++) 
	{
		dofs[i]->setValue(channels[i].evaluate(t));
	}
}

bool Animation::Load(const char *file)
{
	Parser reader;
	reader.Open(file);
	int index = 0;
	reader.FindToken("range");
	rangeMin = reader.GetFloat();
	rangeMax = reader.GetFloat();
	while (reader.FindToken("channel"))
	{
		channel *temp = new channel();
		temp->Load(reader);
		temp->number = index;
		index++;
		channels.push_back(*temp);
	}
	reader.Close();
	return true;
}

void Animation::preCompute()
{
	for (int i = 0; i < channels.size(); i++)
	{
		channels[i].preLoad();
	}
}

Animation::~Animation()
{
}
