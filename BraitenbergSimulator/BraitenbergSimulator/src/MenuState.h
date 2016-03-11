#pragma once

#include "SimState.h"
#include "GUIRenderer.h"

#include <nanovg\nanovg.h>
#include <nanovg\nanovg_gl.h>

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
	UIState ui;
	const float menuWidth = 0.4f;
	const float menuHeight = 0.2f;
	GUIRendererPtr guiRenderer;
	int scroll = 0;
};