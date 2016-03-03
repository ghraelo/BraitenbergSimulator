#include "SimEngine.h"
#include "SimState.h"
#include "MenuState.h"
#include <exception>
#include <memory>

SimEngine::SimEngine()
{
}

void SimEngine::Init()
{
	//initialise main window
	glfwInit();
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	try
	{
		mainWindow = glfwCreateWindow(800, 600, "Braitenberg Simulator", nullptr, nullptr);
		if (mainWindow == nullptr)
			throw std::exception("Failed to create GLFW window\n");
	}
	catch(std::exception& ex)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(mainWindow);

	//glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		throw std::exception("Failed to initialize GLEW");
	}
	//load resources
	//add menu state
	states.push_back(std::make_unique <MenuState>());

	states.front()->Init();
}

void SimEngine::HandleEvents()
{
	double xd, yd;
	glfwGetCursorPos(mainWindow, &xd, &yd);
	mouseState.xPos = (float)xd;
	mouseState.yPos = (float)yd;

	int leftButton = glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_LEFT);
	int rightButton = glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_RIGHT);

	mouseState.leftMouse = leftButton;
	mouseState.rightMouse = rightButton;

	int width;
	int height;
	glfwGetWindowSize(mainWindow, &width, &height);
	windowState.width = width;
	windowState.height = height;
}

void SimEngine::Update()
{
	states.front()->Update(*this);
}

void SimEngine::Render()
{
	glViewport(0, 0, (int)windowState.width, (int)windowState.height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	states.front()->Draw(*this);

	glfwPollEvents();
	glfwSwapBuffers(mainWindow);
}

void SimEngine::Exit()
{
	glfwSetWindowShouldClose(mainWindow, true);
}

void SimEngine::Cleanup()
{
	states.front()->Cleanup();
	glfwTerminate();
}

void SimEngine::ChangeState(SimStatePtr& state)
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(std::move(state));
	states.back()->Init();
}

void SimEngine::PushState(SimStatePtr& state)
{
	// store and init the new state
	states.push_back(std::move(state));
	states.back()->Init();
}

void SimEngine::PopState()
{
	// cleanup the current state
	if (!states.empty()) {
		states.back()->Cleanup();
		states.pop_back();
	}
}

MouseState SimEngine::GetMouseState()
{
	return mouseState;
}

WindowState SimEngine::GetWindowState()
{
	return windowState;
}

GLFWwindow * SimEngine::GetWindow()
{
	return mainWindow;
}
