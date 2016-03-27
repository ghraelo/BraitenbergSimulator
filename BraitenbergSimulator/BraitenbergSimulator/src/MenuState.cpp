#include "MenuState.h"
#include "MainState.h"
#include "NoVisualisationState.h"

#include "SimEngine.h"
#include <glew\glew.h>
#include "imgui.h"

MenuState::MenuState()
{
}

void MenuState::Init(SimEngine & se)
{
	nextState = NS_MenuState;
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

	imguiBeginFrame((int)se.GetMouseState().xPos, (int)se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);


	float x = se.GetWindowState().width / 2 - (menuWidth * se.GetWindowState().width) / 2;
	float y = se.GetWindowState().height / 2 - (menuWidth * se.GetWindowState().height) / 2;

	imguiBeginScrollArea("Main Menu", (int)x, (int)y, (int)(menuWidth * se.GetWindowState().width), (int)(menuHeight * se.GetWindowState().height), &scroll);
	if (imguiButton("Run simulation", true))
	{
		//set flag
		nextState = NS_MainState;
	}
	if (imguiButton("Run simulations (no visualisation)",true))
	{
		nextState = NS_NoVisualisationState;
	}
	if (imguiButton("Exit", true))
	{
		glfwSetWindowShouldClose(se.GetWindow(), true);
	}
	imguiEndScrollArea();
	imguiEndFrame();

	guiRenderer.Flush(vg);
	SimStatePtr p;

	switch (nextState)
	{
	case NS_MainState:
		p = std::make_unique<MainState>();
		ChangeState(se, p);
		break;
	case NS_NoVisualisationState:
		p = std::make_unique<NoVisualisationState>();
		ChangeState(se, p);
		break;
	}

}

void MenuState::HandleEvents(SimEngine & se)
{
}

void MenuState::OnScroll(double scrollOffset)
{
}
