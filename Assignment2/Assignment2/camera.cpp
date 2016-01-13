
#include "camera.h"


Camera::Camera() {
	Reset();
}


void Camera::Update() {
}

void Camera::Reset() {
	FOV=60.0f;
	Aspect=1.33f;
	NearClip=0.1f;
	FarClip=100.0f;

	Distance=5.0f;
	Azimuth=0.0f;
	Incline=0.0f;
}
void Camera::Draw() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(FOV,Aspect,NearClip,FarClip);

	glTranslatef(0,0,-Distance);
	glRotatef(Incline,1.0f,0.0f,0.0f);
	glRotatef(Azimuth,0.0f,1.0f,0.0f);

	glMatrixMode(GL_MODELVIEW);
}
