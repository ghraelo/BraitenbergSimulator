#pragma once

//GLEW
#include <glew\glew.h>
//GLFW
#include <glfw\glfw3.h>

//STL
#include <vector>

#include "SimState.h"
#include "OpenGlRenderer.h"
#include "ResourceManager.h"

struct MouseState
{
	float xPos;
	float yPos;
	int leftMouse;
	int rightMouse;
};

struct WindowState
{
	float width;
	float height;
};

class SimEngine
{
public:
	SimEngine();
	void Init();
	void HandleEvents();
	void Update();
	void Render();
	void Exit();
	void Cleanup();
	void ChangeState(SimStatePtr& state);
	void PushState(SimStatePtr& state);
	void PopState();

	MouseState GetMouseState();
	WindowState GetWindowState();
	GLFWwindow* GetWindow();
	OpenGLRenderer renderer;
private:
	GLFWwindow* mainWindow;
	std::vector<SimStatePtr> states;
	MouseState mouseState;
	WindowState windowState;
};