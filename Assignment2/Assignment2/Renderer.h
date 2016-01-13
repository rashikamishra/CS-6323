#ifndef UTILS_H
#include "Utils.h"
#endif

#include "camera.h"

class Renderer {
public:
	Renderer(int argc, char **argv);
	~Renderer();

	void Update();
	void Reset();
	void Draw();
	void Color();
	void Solid();
	void OpenNewSkeleton();
	void RenderSkeleton();
	void ShowAxis();
	void Quit();
	void ShowHelp();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);


private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Components
	Camera Cam;
};
