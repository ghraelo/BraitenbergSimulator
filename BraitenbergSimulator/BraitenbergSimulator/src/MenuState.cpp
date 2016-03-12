#include "MenuState.h"
#include "MainState.h"

#include "SimEngine.h"
#include <glew\glew.h>
#include "imgui.h"

MenuState::MenuState()
{
}

void MenuState::Init(SimEngine & se)
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
	NVGcontext* vg = se.GetContext();

	bool changing = false;

	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);


	float x = se.GetWindowState().width / 2 - (menuWidth * se.GetWindowState().width) / 2;
	float y = se.GetWindowState().height / 2 - (menuWidth * se.GetWindowState().height) / 2;

	imguiBeginScrollArea("Main Menu", x, y, (menuWidth * se.GetWindowState().width), (menuHeight * se.GetWindowState().height), &scroll);
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

	guiRenderer.Flush(vg);
	if (changing)
	{
		SimStatePtr p = std::make_unique<MainState>();
		ChangeState(se, p);
	}

}
