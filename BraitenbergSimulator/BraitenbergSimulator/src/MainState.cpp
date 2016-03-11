#include "MainState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"

MainState::MainState()
{
}

void MainState::Init()
{
	std::vector<std::string> out;
	ResourceManager::GetFilesInDirectory(out, "yaml");
	if (out.size() > 0)
	{
		m_baseSettings.activeSceneFilename = out[0];
	}
	else
	{
		printf("error: no scene files found\n");
	}
}

void MainState::Cleanup()
{
}

void MainState::Update(SimEngine & se)
{
	if (m_baseSettings.shouldExit)
	{
		SimStatePtr p = std::make_unique<MenuState>();
		ChangeState(se, p);
	}
}

void MainState::Draw(SimEngine & se)
{
	NVGcontext* vg = se.GetContext();
	// draw interface here
	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
	uim.DrawBaseUI(m_baseSettings, se.GetWindowState());
	imguiEndFrame();

	guiRenderer.Flush(vg);
}
