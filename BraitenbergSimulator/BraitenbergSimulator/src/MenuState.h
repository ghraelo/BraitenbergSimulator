#pragma once

#include "SimState.h"
#include "OpenGLRenderer.h"

#include "imgui.h"
#include "RenderGL3.h"

struct UIState
{
	bool showMenu;
	int scroll;
	int scrollarea1;
	bool mouseOverMenu;
	bool chooseTest;
};

class MenuState : public SimState
{
public:
	MenuState();
	void Init() override;
	void Cleanup() override;
	void Update(SimEngine& se) override;
	void Draw(SimEngine& se) override;

private:
	void Interface(SimEngine & se);
	UIState ui;
	const float menuWidth = 0.4f;
	const float menuHeight = 0.2f;
};