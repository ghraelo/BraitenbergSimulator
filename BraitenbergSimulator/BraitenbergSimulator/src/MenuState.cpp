#include "MenuState.h"
#include "MainState.h"

#include "SimEngine.h"
#include <glew\glew.h>
MenuState::MenuState()
{
}

void MenuState::Init()
{
	ui.showMenu = true;
	ui.scroll = 0;
	ui.scrollarea1 = 0;
	ui.chooseTest = false;
	ui.mouseOverMenu = false;

	// Init UI
	const char* fontPath = "Data/DroidSans.ttf";
	if (RenderGLInit(fontPath) == false)
	{
		fprintf(stderr, "Could not init GUI renderer.\n");
		assert(false);
		return;
	}
}

void MenuState::Cleanup()
{
	RenderGLDestroy();
}

void MenuState::Update(SimEngine & se)
{
}

void MenuState::Draw(SimEngine & se)
{
	unsigned char mousebutton = 0;
	int mscroll = ui.scroll;
	ui.scroll = 0;

	if (se.GetMouseState().leftMouse == GLFW_PRESS)
	{
		mousebutton |= IMGUI_MBUT_LEFT;
	}
	imguiBeginFrame(se.GetMouseState().xPos, se.GetWindowState().height - se.GetMouseState().yPos, mousebutton, mscroll);
	Interface(se);
	imguiEndFrame();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	RenderGLFlush(se.GetWindowState().width, se.GetWindowState().height);
}

void MenuState::Interface(SimEngine & se)
{
	int x = (1 - menuWidth) * se.GetWindowState().width / 2;
	int y = se.GetWindowState().height - ((1 + menuHeight) * se.GetWindowState().height / 2);
	int w = se.GetWindowState().width * menuWidth;
	int h = se.GetWindowState().height * menuHeight;

	bool over = imguiBeginScrollArea("Main Menu", x, y, w, h, &ui.scrollarea1);
	if (over) ui.mouseOverMenu = true;
	imguiSeparatorLine();

	if (imguiButton("Start simulation", true))
	{
		SimStatePtr p = std::make_unique<MainState>();
		ChangeState(se, p);
	}

	if (imguiButton("Exit", true))
	{
		se.Exit();
	}
	imguiEndScrollArea();
}
