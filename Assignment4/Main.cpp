//Code reused from AntTweakBar examples

#include <AntTweakBar.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include "Skeleton.h"
#include "skin.h"
#include "Animation.h"
#include <windows.h>
#include <time.h>

typedef enum { WASP = 1 } Shape;
#define NUM_SHAPES 2
Shape g_CurrentShape = WASP;

float g_Zoom = 1.0f;
float g_Rotation[] = { 0.0f, 0.0f, 0.0f, 1.0f };

int g_AutoRotate = 0;
int g_RotateTime = 0;

float g_RotateStart[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float g_MatAmbient1[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float g_MatDiffuse1[] = { 1.0f, 1.0f, 0.0f, 1.0f };

float g_LightMultiplier = 1.0f;
float g_LightDirection[] = { -0.57735f, -0.57735f, -0.57735f };
float g_LightDirection2[] = { 0.57735f, 0.57735f, 0.57735f };

Skeleton *skeletonWasp;
Animation *animation;
skin *skinningWasp;

int index = 0;
float newTime = -4;
float oldTime = 0;
bool animate = false;

time_t prevTime = time(NULL);
time_t currentTime = time(NULL);

int winW, winH;

void SetQuaternionFromAxisAngle(const float *axis, float angle, float *quat)
{
	float sina2, norm;
	sina2 = (float)sin(0.5f * angle);
	norm = (float)sqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
	quat[0] = sina2 * axis[0] / norm;
	quat[1] = sina2 * axis[1] / norm;
	quat[2] = sina2 * axis[2] / norm;
	quat[3] = (float)cos(0.5f * angle);
}

void ConvertQuaternionToMatrix(const float *quat, float *mat)
{
	float yy2 = 2.0f * quat[1] * quat[1];
	float xy2 = 2.0f * quat[0] * quat[1];
	float xz2 = 2.0f * quat[0] * quat[2];
	float yz2 = 2.0f * quat[1] * quat[2];
	float zz2 = 2.0f * quat[2] * quat[2];
	float wz2 = 2.0f * quat[3] * quat[2];
	float wy2 = 2.0f * quat[3] * quat[1];
	float wx2 = 2.0f * quat[3] * quat[0];
	float xx2 = 2.0f * quat[0] * quat[0];
	mat[0 * 4 + 0] = -yy2 - zz2 + 1.0f;
	mat[0 * 4 + 1] = xy2 + wz2;
	mat[0 * 4 + 2] = xz2 - wy2;
	mat[0 * 4 + 3] = 0;
	mat[1 * 4 + 0] = xy2 - wz2;
	mat[1 * 4 + 1] = -xx2 - zz2 + 1.0f;
	mat[1 * 4 + 2] = yz2 + wx2;
	mat[1 * 4 + 3] = 0;
	mat[2 * 4 + 0] = xz2 + wy2;
	mat[2 * 4 + 1] = yz2 - wx2;
	mat[2 * 4 + 2] = -xx2 - yy2 + 1.0f;
	mat[2 * 4 + 3] = 0;
	mat[3 * 4 + 0] = mat[3 * 4 + 1] = mat[3 * 4 + 2] = 0;
	mat[3 * 4 + 3] = 1;
}


void MultiplyQuaternions(const float *q1, const float *q2, float *qout)
{
	float qr[4];
	qr[0] = q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1];
	qr[1] = q1[3] * q2[1] + q1[1] * q2[3] + q1[2] * q2[0] - q1[0] * q2[2];
	qr[2] = q1[3] * q2[2] + q1[2] * q2[3] + q1[0] * q2[1] - q1[1] * q2[0];
	qr[3] = q1[3] * q2[3] - (q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2]);
	qout[0] = qr[0]; qout[1] = qr[1]; qout[2] = qr[2]; qout[3] = qr[3];
}


int GetTimeMs()
{
#if !defined(_WIN32)
	return glutGet(GLUT_ELAPSED_TIME);
#else
	return (int)GetTickCount();
#endif
}

void orthogonalStart(void) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, winW, 0, winH);
	glScalef(1, -1, 1);
	glTranslatef(0, -winH, 0);
	glMatrixMode(GL_MODELVIEW);
}

void orthogonalEnd(void) {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

GLuint LoadTexture(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


	//even better quality, but this will do for now.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//to the edge of our shape. 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data); //free the texture
	return texture; //return whether it was successful
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}
void background(void) {
	glBindTexture(GL_TEXTURE_2D, LoadTexture("grass.bmp", winW, winH));

	//orthogonalStart();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	


	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2f(0, 0);
	glTexCoord2d(1.0, 0.0); glVertex2f(1024, 0);
	glTexCoord2d(1.0, 1.0); glVertex2f(1024, 1024);
	glTexCoord2d(0.0, 1.0); glVertex2f(0, 1024);
	glEnd();

	//orthogonalEnd();
}
void Display(void)
{
	//display the background image here
	
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(0, 0);
	glDrawPixels()*/

	/*
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	*/
	//background();


		float v[4]; 
		float v1[4];
		float mat[4 * 4];

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_NORMALIZE);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		v[0] = v[1] = v[2] = g_LightMultiplier*0.4f; v[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_AMBIENT, v);
		v[0] = v[1] = v[2] = g_LightMultiplier*0.8f; v[3] = 1.0f;
		glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
		v[0] = -g_LightDirection[0]; v[1] = -g_LightDirection[1]; v[2] = -g_LightDirection[2]; v[3] = 0.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, v);
		

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
		v1[0] = v1[1] = v1[2] = g_LightMultiplier*0.4f; v1[3] = 1.0f;
		glLightfv(GL_LIGHT1, GL_AMBIENT, v1);
		v1[0] = v1[1] = v1[2] = g_LightMultiplier*0.8f; v1[3] = 1.0f;
		glLightfv(GL_LIGHT1, GL_DIFFUSE, v1);
		v1[0] = -g_LightDirection2[0]; v1[1] = -g_LightDirection2[1]; v1[2] = -g_LightDirection2[2]; v1[3] = 0.0f;
		glLightfv(GL_LIGHT1, GL_POSITION, v1);

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, g_MatAmbient1);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, g_MatDiffuse1);

		glPushMatrix();
		glTranslatef(0.5f, -0.3f, 0.0f);
		if (g_AutoRotate)
		{
			float axis[3] = { 0, 1, 0 };
			float angle = (float)(GetTimeMs() - g_RotateTime) / 1000.0f;
			float quat[4];
			SetQuaternionFromAxisAngle(axis, angle, quat);
			MultiplyQuaternions(g_RotateStart, quat, g_Rotation);
		}
		ConvertQuaternionToMatrix(g_Rotation, mat);
		glMultMatrixf(mat);
		glScalef(g_Zoom, g_Zoom, g_Zoom);
		Matrices identity;
		animation->evaluate(newTime);
		if (animate){
			float timeDiff = currentTime - prevTime;
			prevTime = currentTime;
			currentTime = time(NULL);
			newTime = newTime + 0.01;
		}
		skeletonWasp->calculate(identity.IDENTITY);
		skinningWasp->update();
		skinningWasp->draw();		
		glPopMatrix();
	

	TwDraw();

	glutSwapBuffers();

	glutPostRedisplay();
}


void Reshape(int width, int height)
{
	winW = width;
	winH = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)width / height, 1, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
	glTranslatef(0, 0.6f, -1);

	TwWindowSize(width, height);
}


void Terminate(void)
{
	glFinish();
	exit(0);
}


void TW_CALL SetAutoRotateCB(const void *value, void *clientData)
{

	g_AutoRotate = *(const int *)value;
	if (g_AutoRotate != 0)
	{
		g_RotateTime = GetTimeMs();
		g_RotateStart[0] = g_Rotation[0];
		g_RotateStart[1] = g_Rotation[1];
		g_RotateStart[2] = g_Rotation[2];
		g_RotateStart[3] = g_Rotation[3];
		TwDefine(" TweakBar/ObjRotation readonly ");
	}
	else
		TwDefine(" TweakBar/ObjRotation readwrite ");
}

void TW_CALL GetAutoRotateCB(void *value, void *clientData)
{
	*(int *)value = g_AutoRotate; 
}

void CreateBarLabels(TwBar *skeletonBar, Skeleton *skeleton)
{
	DOF* currentJointX;
	DOF* currentJointY;
	DOF* currentJointZ;
	float vstep = 0.1f;
	if (skeleton->joints.size() != 0)
	{
		for (std::vector<DOF*>::iterator i = skeleton->joints.begin() + 3; i != skeleton->joints.end(); i += 3)
		{
			currentJointX = *i;
			currentJointY = *(i + 1);
			currentJointZ = *(i + 2);

			std::string name = currentJointX->getName() + "XRotation";

			TwAddVarRW(skeletonBar, name.c_str(), TW_TYPE_FLOAT, &currentJointX->value, "opened=true Help 'Rotates in XDirection'");
			TwSetParam(skeletonBar, name.c_str(), "group", TW_PARAM_CSTRING, 1, currentJointX->getName().c_str());
			TwSetParam(skeletonBar, name.c_str(), "min", TW_PARAM_FLOAT, 1, &currentJointX->min);
			TwSetParam(skeletonBar, name.c_str(), "max", TW_PARAM_FLOAT, 1, &currentJointX->max);
			TwSetParam(skeletonBar, name.c_str(), "step", TW_PARAM_FLOAT, 1, &vstep);


			name = currentJointX->getName() + "YRotation";
			TwAddVarRW(skeletonBar, name.c_str(), TW_TYPE_FLOAT, &currentJointY->value, "Help 'Rotates in YDirection'");
			TwSetParam(skeletonBar, name.c_str(), "group", TW_PARAM_CSTRING, 1, currentJointY->getName().c_str());
			TwSetParam(skeletonBar, name.c_str(), "min", TW_PARAM_FLOAT, 1, &currentJointY->min);
			TwSetParam(skeletonBar, name.c_str(), "max", TW_PARAM_FLOAT, 1, &currentJointY->max);
			TwSetParam(skeletonBar, name.c_str(), "step", TW_PARAM_FLOAT, 1, &vstep);

			name = currentJointX->getName() + "ZRotation";
			TwAddVarRW(skeletonBar, name.c_str(), TW_TYPE_FLOAT, &currentJointZ->value, "Help 'Rotates in ZDirection'");
			TwSetParam(skeletonBar, name.c_str(), "group", TW_PARAM_CSTRING, 1, currentJointZ->getName().c_str());
			TwSetParam(skeletonBar, name.c_str(), "min", TW_PARAM_FLOAT, 1, &currentJointZ->min);
			TwSetParam(skeletonBar, name.c_str(), "max", TW_PARAM_FLOAT, 1, &currentJointZ->max);
			TwSetParam(skeletonBar, name.c_str(), "step", TW_PARAM_FLOAT, 1, &vstep);
		}
	}
}

void CreateBar()
{
	TwBar *waspskeletonBar = TwNewBar("WaspSkeletonBar");
	
	TwDefine(" GLOBAL help='This is to rotate a single bone.' ");
	TwDefine(" WaspSkeletonBar size='200 400' color='96 216 224' ");
	
	CreateBarLabels(waspskeletonBar, skeletonWasp);
	
}

void TW_CALL ResetValues(void * /*clientData*/)
{

	//g_CurrentShape = WASP;
	g_Zoom = 1.0f;

	g_Rotation[0] = 0.0f;
	g_Rotation[1] = 0.0f;
	g_Rotation[2] = 0.0f;
	g_Rotation[3] = 1.0f;

	g_AutoRotate = 0;
	g_RotateTime = 0;

	g_RotateStart[0] = 0.0f;
	g_RotateStart[1] = 0.0f;
	g_RotateStart[2] = 0.0f;
	g_RotateStart[3] = 1.0f;

	g_MatAmbient1[0] = 0.5f;
	g_MatAmbient1[1] = 0.0f;
	g_MatAmbient1[2] = 0.0f;
	g_MatAmbient1[3] = 1.0f;

	g_MatDiffuse1[0] = 1.0f;
	g_MatDiffuse1[1] = 1.0f;
	g_MatDiffuse1[2] = 0.0f;
	g_MatDiffuse1[3] = 1.0f;

	g_LightMultiplier = 1.0f;

	g_LightDirection[0] = -0.57735f;
	g_LightDirection[1] = -0.57735f;
	g_LightDirection[2] = -0.57735f;

	g_LightDirection2[0] = 0.57735f;
	g_LightDirection2[1] = 0.57735f;
	g_LightDirection2[2] = 0.57735f;
	newTime = -4;
	animate = false;
	skeletonWasp->reset();
	//skeletonTube->reset();

	Display();
}

void TW_CALL StartAnimation(void * /*clientData*/)
{
	animate = !animate;
	

	Display();
}


int main(int argc, char *argv[])
{
	TwBar *bar; 
	float axis[] = { 0.7f, 0.7f, 0.0f }; 
	float angle = 0.8f;
	winW = 640;
	winH = 480;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Assignment3- Skinning");
	glutCreateMenu(NULL);

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	atexit(Terminate);  

	TwInit(TW_OPENGL, NULL);

	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);

	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);

	//Todo
	Matrices identity;
	skeletonWasp = new Skeleton("wasp_walk.skel");
	skinningWasp = new skin("wasp_walk.skin", skeletonWasp->worldMatrixes);
	animation = new Animation("wasp_walk.anim", skeletonWasp->joints);
	skeletonWasp->calculate(identity.IDENTITY);
	skinningWasp->update();
	
	bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This is rotate and zoom the skeleton also to change the lighting effects' ");
	TwDefine(" TweakBar size='200 400' color='96 216 224' ");

	TwAddButton(bar, "Reset", ResetValues, NULL, " label='Resets the View' ");
	TwAddButton(bar, "Animate", StartAnimation, NULL, " label='Starts the animation' ");
	TwAddVarRW(bar, "Zoom", TW_TYPE_FLOAT, &g_Zoom,
		" min=0.01 max=2.5 step=0.01 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ");
	TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation,
		" label='Object rotation' opened=true help='Change the object orientation.' ");

	TwAddVarCB(bar, "AutoRotate", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL,
		" label='Auto-rotate' key=space help='Toggle auto-rotate mode.' ");
	TwAddVarRW(bar, "Multiplier", TW_TYPE_FLOAT, &g_LightMultiplier,
		" label='Light booster' min=0.1 max=4 step=0.02 keyIncr='+' keyDecr='-' help='Increase/decrease the light power.' ");
	TwAddVarRW(bar, "LightDir", TW_TYPE_DIR3F, &g_LightDirection,
		" label='Light direction' opened=true help='Change the light direction.' ");
	TwAddVarRW(bar, "LightDir2", TW_TYPE_DIR3F, &g_LightDirection2,
		" label='Light direction' opened=true help='Change the light direction.' ");

	TwAddVarRW(bar, "Ambient", TW_TYPE_COLOR3F, &g_MatAmbient1, " group='Material' ");

	TwAddVarRW(bar, "Diffuse", TW_TYPE_COLOR3F, &g_MatDiffuse1, " group='Material' ");

	{
		TwEnumVal shapeEV[NUM_SHAPES] = { { WASP, "Wasp" } };
		TwType shapeType = TwDefineEnum("ShapeType", shapeEV, NUM_SHAPES);
		TwAddVarRW(bar, "Shape", shapeType, &g_CurrentShape, " keyIncr='<' keyDecr='>' help='Change object shape.' ");
	}

	g_RotateTime = GetTimeMs();
	SetQuaternionFromAxisAngle(axis, angle, g_Rotation);
	SetQuaternionFromAxisAngle(axis, angle, g_RotateStart);
	CreateBar();
	glutMainLoop();

	return 0;
}

