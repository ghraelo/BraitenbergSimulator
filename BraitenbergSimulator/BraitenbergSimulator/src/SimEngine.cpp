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
		mainWindow = glfwCreateWindow(1024, 720, "Braitenberg Simulator", nullptr, nullptr);
		if (mainWindow == nullptr)
			throw std::exception("Failed to create GLFW window\n");
		windowState.width = 1024;
		windowState.height = 720;
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

	nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
	//load resources
	//add menu state
	states.push_back(std::make_unique <MenuState>());

	states.front()->Init(*this);

	//set up font
	int fontHandle = nvgCreateFont(nvg, "droidsans", "Data/DroidSans.ttf");
	nvgFontFace(nvg, "droidsans");
	nvgFontSize(nvg, 10);
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

void SimEngine::Update(double frameTime)
{
	m_frameTime = frameTime;
	states.front()->Update(*this);
}

void SimEngine::Render()
{
	glViewport(0, 0, (int)windowState.width, (int)windowState.height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(nvg, windowState.width, windowState.height,1.0);
	states.front()->Draw(*this);
	nvgEndFrame(nvg);
}

void SimEngine::Exit()
{
	glfwSetWindowShouldClose(mainWindow, true);
}

void SimEngine::Cleanup()
{
	states.front()->Cleanup();
	nvgDeleteGL3(nvg);
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
	states.back()->Init(*this);
}

void SimEngine::PushState(SimStatePtr& state)
{
	// store and init the new state
	states.push_back(std::move(state));
	states.back()->Init(*this);
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

NVGcontext * SimEngine::GetContext()
{
	return nvg;
}

double SimEngine::GetFrameTime()
{
	return m_frameTime;
}
