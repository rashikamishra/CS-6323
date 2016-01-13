#ifndef UTILS_H
#define UTILS_H
#endif


#ifdef WIN32
#define M_PI	3.14159f
#include <windows.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <GL/freeglut.h>
class Utils
{
public:
	void drawAxis(float size);
	void drawWireBox(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
	void SetIsColor();
	void SetIsSolid();
	void ShowAxis();
private:
	 bool IsColored = true;
	 bool IsSolid = false;
	 bool IsShowAxis = true;


};