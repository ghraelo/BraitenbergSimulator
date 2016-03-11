#pragma once

#include "SimState.h"
#include "BaseUISettings.h"
#include "UIManager.h"
#include "GUIRenderer.h"

//base UI settings (passed to/from UI manager)

class MainState : public SimState
{
public:
	MainState();
	void Init() override;
	void Cleanup() override;
	void Update(SimEngine& se) override;
	void Draw(SimEngine& se) override;
private:
	UIManager uim;
	BaseUISettings m_baseSettings;
	GUIRenderer guiRenderer;
};