#ifndef UTILS_H
#include "Utils.h"
#endif
void Utils::drawAxis(float size) {
	if (IsShowAxis)
	{
		glBegin(GL_LINES);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(size, 0.0f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, size, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, size);

		glColor4f(0.5f, 0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-size, 0.0f, 0.0f);

		glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, -size, 0.0f);

		glColor4f(0.0f, 0.0f, 0.5f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -size);	
		glEnd();
	}
}


void Utils::drawWireBox(float xmin,float ymin,float zmin,float xmax,float ymax,float zmax) {
	glPushMatrix();
		glTranslatef(0.5f*(xmin+xmax),0.5f*(ymin+ymax),0.5f*(zmin+zmax));
		glScalef(xmax-xmin,ymax-ymin,zmax-zmin);
		if (IsColored)
			glColor4f(xmax - xmin, ymax - ymin, zmax - zmin, 1.0f);
		else
			glColor4f(1.0f,1.0f,1.0f, 1.0f);
		if (IsSolid)
			glutSolidCube(1.0f);
		else
			glutWireCube(1.0f);
	glPopMatrix();
}

void Utils::ShowAxis()
{
	if (IsShowAxis)
		IsShowAxis = false;
	else
		IsShowAxis = true;
}

void Utils::SetIsColor()
{
	if (IsColored)
		IsColored = false;
	else
		IsColored = true;
}
void Utils::SetIsSolid()
{
	if (IsSolid)
		IsSolid = false;
	else
		IsSolid = true;
}