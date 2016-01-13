
#include "Renderer.h"
#include "Parser.h"
#include "OpenFileDialog.h"
#include <string>
#include "SkeletalModel.h"
#define WINDOWTITLE	"Skeleton"


static Renderer *renderer;

static SkeletalModel  *skeleton;

static Utils *utils;

static std::string ShowFileDialog()
{
	OpenFileDialog* openFileDialog1 = new OpenFileDialog();

	openFileDialog1->FilterIndex = 1;	
	openFileDialog1->InitialDir = _T("..\\..");
	openFileDialog1->Title = _T("Open Text File");
	openFileDialog1->DefaultExtension = ".skel";

	if (!openFileDialog1->ShowDialog())
	{
		MessageBox(0, openFileDialog1->FileName, "Error",
			MB_OK | MB_ICONINFORMATION);
	}

	return openFileDialog1->FileName;

}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	
	renderer = new Renderer(argc, argv);

	glutMainLoop();
	return 0;
}

static void display()									{renderer->Draw();}
static void idle()										{renderer->Update();}
static void resize(int x,int y)							{renderer->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{renderer->Keyboard(key,x,y);}
static void mousebutton(int btn,int state,int x,int y)	{renderer->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{renderer->MouseMotion(x,y);}

Renderer::Renderer(int argc, char **argv) {
	OpenNewSkeleton();
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );
	glutSetWindow( WindowHandle );
	
	glClearColor( 0., 0., 0., 1. );

	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));
	
}



Renderer::~Renderer() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

void Renderer::Update() {

	Cam.Update();

	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}


void Renderer::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
}


void Renderer::Color() {
	utils->SetIsColor();
	Draw();
}

void Renderer::OpenNewSkeleton() {
	std::string filename = ShowFileDialog();
	skeleton = new SkeletalModel();
	skeleton->load(filename.c_str());

	utils = new Utils();
	
}

void Renderer::ShowHelp()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WinX, 0, WinY, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	char *CommandString = "Commands :";
	
	glRasterPos2f(10, 200);
	
	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)CommandString);

	char *QuitString = "q : Quit ";

	glRasterPos2f(10, 185);

	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)QuitString);

	char *ResetString = "r : Reset ";

	glRasterPos2f(10, 170);

	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)ResetString);

	char *ColorString = "c : Colored/White ";

	glRasterPos2f(10, 155);

	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)ColorString);

	char *SolidString = "s : Solid/Wire Box ";

	glRasterPos2f(10, 140);

	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)SolidString);

	char *AxisString = "a : Show/Hide Axis ";

	glRasterPos2f(10, 125);

	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)AxisString);

	char *OpenString = "o : Render New Skeleton ";

	glRasterPos2f(10, 110);

	glutBitmapString(GLUT_BITMAP_9_BY_15, (const unsigned char*)OpenString);


	glPopAttrib();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	
}

void Renderer::ShowAxis()
{
	utils->ShowAxis();
	
	Draw();
}

void Renderer::RenderSkeleton()
{
	OpenNewSkeleton();
	Reset();
	Draw();
}

void Renderer::Solid() {
	utils->SetIsSolid();
	Draw();
}


void Renderer::Draw() {

	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Draw components
	Cam.Draw();		
	
	utils->drawAxis(512.0f);

	skeleton->drawSkeleton(utils);
	
	glFinish();
	ShowHelp();
	glutSwapBuffers();
}



void Renderer::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}


void Renderer::Resize(int x, int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}


void Renderer::Keyboard(int key, int x, int y) {
	switch(key) {
		case 'q':		
			Quit();
			break;
		case 'r':
			Reset();
			break;
		case 'c':
			Color();
			break;
		case 's':
			Solid();
			break;
		case 'o':
			RenderSkeleton();
			break;
		case 'a':
			ShowAxis();
			break;
	}
}

void Renderer::MouseButton(int btn, int state, int x, int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

void Renderer::MouseMotion(int nx, int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}
