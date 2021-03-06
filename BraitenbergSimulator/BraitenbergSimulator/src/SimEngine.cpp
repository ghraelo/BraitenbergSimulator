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

	//set callbacks
	glfwSetWindowUserPointer(mainWindow, this);

	auto scrollFunc = [](GLFWwindow* w, double xoffset, double yoffset)
	{
		static_cast<SimEngine*>(glfwGetWindowUserPointer(w))->OnScroll(yoffset);
	};

	glfwSetScrollCallback(mainWindow, scrollFunc);

	auto mouseFunc = [](GLFWwindow* w, int button, int action, int mods)
	{
		if(action == GLFW_PRESS)
			static_cast<SimEngine*>(glfwGetWindowUserPointer(w))->OnMouseDown(button);
		else if(action == GLFW_RELEASE)
			static_cast<SimEngine*>(glfwGetWindowUserPointer(w))->OnMouseUp(button);
	};

	glfwSetMouseButtonCallback(mainWindow, mouseFunc);
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

	states.front()->HandleEvents(*this);
}

void SimEngine::Update(double frameTime)
{
	m_frameTime = frameTime;
	double u0 = glfwGetTime();
	states.front()->Update(*this);
	double u1 = glfwGetTime();
	m_updateTime = u1 - u0;
}

void SimEngine::Render()
{
	double u0 = glfwGetTime();
	glViewport(0, 0, (int)windowState.width, (int)windowState.height);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	
	nvgBeginFrame(nvg, windowState.width, windowState.height,1.0);
	states.front()->Draw(*this);
	nvgEndFrame(nvg);
	double u1 = glfwGetTime();
	m_drawTime = u1 - u0;
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

double SimEngine::GetUpdateTime()
{
	return m_updateTime;
}

double SimEngine::GetDrawTime()
{
	return m_drawTime;
}

void SimEngine::OnScroll(double yoffset)
{
	mouseState.scrollOffset = yoffset;
	//printf("scrollOffset: %f\n", yoffset);
	states.front()->OnScroll(yoffset);
}

void SimEngine::OnMouseDown(int button)
{
}

void SimEngine::OnMouseUp(int button)
{
}
