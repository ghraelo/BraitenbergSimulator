#pragma once

//GLEW
#include <glew\glew.h>
//GLFW
#include <glfw\glfw3.h>
//NanoVG
#include <nanovg\nanovg.h>
#include <nanovg\nanovg_gl.h>


//STL
#include <vector>

#include "SimState.h"
#include "WMState.h"

class SimEngine
{
public:
	SimEngine();
	void Init();
	void HandleEvents();
	void Update(double frameTime);
	void Render();
	void Exit();
	void Cleanup();
	void ChangeState(SimStatePtr& state);
	void PushState(SimStatePtr& state);
	void PopState();

	MouseState GetMouseState();
	WindowState GetWindowState();
	GLFWwindow* GetWindow();
	NVGcontext* GetContext();
	double GetFrameTime();
private:
	GLFWwindow* mainWindow;
	std::vector<SimStatePtr> states;
	MouseState mouseState;
	WindowState windowState;
	NVGcontext* nvg;
	double m_frameTime = 0.0;
};