#ifndef CHANNEL_H
#define CHANNE_H
#endif
#include <vector>
#include "keyframe.h"

#ifndef PARSER_H
#include "Parser.h"
#endif
#include <string>
#include <iostream>
class channel
{
private:
	std::vector<keyframe> keyframes;
	
	int lastIndex = 0;
	float length;
	keyframe start;
	keyframe end;

public:
	channel();

	~channel();
	std::string extrapolateInMode;

	std::string extrapolateOutMode;

	int number;

	void preLoad();

	float evaluate(float t);

	bool Load(Parser &reader);
};

