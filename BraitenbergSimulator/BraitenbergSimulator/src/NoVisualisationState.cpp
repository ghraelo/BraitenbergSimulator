#include "NoVisualisationState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"
#include "MathUtils.h"
#include "Statistics.h"


NoVisualisationState::NoVisualisationState()
	:sm(30)
{

}

void NoVisualisationState::Init(SimEngine & se)
{
	//set default scene file
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

	ScenePtr s = ResourceManager::LoadScene(m_baseSettings.activeSceneFilename);

	simThread = std::make_unique<SimulationThread>(std::move(s), 60000);
	m_baseSettings.running = true;

	m_baseSettings.startTime = glfwGetTime();

	prevMouseState = se.GetMouseState();

	printf("number of cores: %s\n", getenv("NUMBER_OF_PROCESSORS"));
}

void NoVisualisationState::Cleanup()
{
	simThread->Exit();
}

void NoVisualisationState::Update(SimEngine & se)
{

	m_baseSettings.frameTime = se.GetFrameTime();
	
	if (m_baseSettings.shouldExit)
	{
		SimStatePtr p = std::make_unique<MenuState>();
		ChangeState(se, p);
		return;
	}

	if (simThread->IsDone())
		m_baseSettings.running = false;

	m_baseSettings.simTime = simThread->GetElapsedTime();
	
	//printf("simTime: %f s\n", simTime);	
}

void NoVisualisationState::Draw(SimEngine & se)
{
	//printf("frame time: %0.2f s\n", se.GetFrameTime());
	//printf("update time: %0.2f s\n", se.GetUpdateTime());
	//printf("draw time: %0.2f s\n", se.GetDrawTime());

	NVGcontext* vg = se.GetContext();

	// draw interface here
	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
	uim.DrawNoVisUI(m_baseSettings, se.GetWindowState());

	imguiEndFrame();

	guiRenderer.Flush(vg);
}

void NoVisualisationState::HandleEvents(SimEngine & se)
{
}

void NoVisualisationState::OnScroll(double scrollOffset)
{
}

void NoVisualisationState::LoadScene(ScenePtr & ptr_scene)
{
	m_currentScene = std::move(ptr_scene);
}
