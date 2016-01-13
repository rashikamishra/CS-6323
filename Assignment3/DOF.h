#ifndef DOF_H
#define DOF_H

#ifndef MATRICES_H
#include "Matrices.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

class DOF
{
	private:
		std::string name;

	public:
		float max;

		float min;

		float value;

		DOF();

		DOF(std::string name);

		~DOF();

		std::string getName();

		void setName(std::string name);

		void setLimit(float maxVal, float minVal);

		void setValue(float val);
};

#endif