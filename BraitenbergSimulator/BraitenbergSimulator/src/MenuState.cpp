#include "MenuState.h"
#include "MainState.h"

#include "SimEngine.h"
#include <glew\glew.h>
#include "imgui.h"

MenuState::MenuState()
{
}

void MenuState::Init()
{

}

void MenuState::Cleanup()
{
}

void MenuState::Update(SimEngine & se)
{
}

void MenuState::Draw(SimEngine & se)
{
	bool changing = false;
	guiRenderer = std::make_unique<GUIRenderer>(se.GetWindowState().width, se.GetWindowState().height, 1.0f);
	guiRenderer->SetFont("Data/DroidSans.ttf");
	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0,guiRenderer.get());


	float x = se.GetWindowState().width / 2 - (menuWidth * se.GetWindowState().width) / 2;
	float y = se.GetWindowState().height / 2 - (menuWidth * se.GetWindowState().height) / 2;

	imguiBeginScrollArea("test", x, y, (menuWidth * se.GetWindowState().width), (menuHeight * se.GetWindowState().height), &scroll);
	if (imguiButton("Run simulation", true))
	{
		//set flag
		changing = true;
	}
	if (imguiButton("Run simulations (no visualisation)",true))
	{
		//TODO
	}
	if (imguiButton("Exit", true))
	{
		glfwSetWindowShouldClose(se.GetWindow(), true);
	}
	imguiEndScrollArea();
	imguiEndFrame();

	guiRenderer->Flush();
	if (changing)
	{
		SimStatePtr p = std::make_unique<MainState>();
		ChangeState(se, p);
	}

}
