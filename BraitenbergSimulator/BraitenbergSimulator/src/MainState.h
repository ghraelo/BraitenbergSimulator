#pragma once

#include "SimState.h"
#include "SceneManager.h"

#include "imgui.h"
#include "RenderGL3.h"

#include "DebugDraw.h"

class MainState : public SimState
{
public:
	MainState();
	void Init() override;
	void Cleanup() override;
	void Update(SimEngine& se) override;
	void Draw(SimEngine& se) override;
private:
	SceneManager sm;
};