#include "MainState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"
#include "MathUtils.h"
#include "Statistics.h"

#include <sstream>

MainState::MainState()
	:sm(30)
{
}

void MainState::Init(SimEngine & se)
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
	activeSceneName = m_baseSettings.activeSceneFilename;

	ScenePtr s = ResourceManager::LoadScene(m_baseSettings.activeSceneFilename);

	simManager = std::make_unique<SimManager>(s);

	cam = std::make_unique<Camera>(se.GetWindowState().width,se.GetWindowState().height);

	m_sceneRenderer.SetCamera(cam.get());

	m_baseSettings.startTime = glfwGetTime();

	BoundaryCallbackFunc func = [&](BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle) {
		int i = 0;
	};

	simManager->GetBoundary()->RegisterCallback(func);

	prevMouseState = se.GetMouseState();
	
	//wrap angle test
}

void MainState::Cleanup()
{

}

void MainState::Update(SimEngine & se)
{
	m_baseSettings.frameTime = se.GetFrameTime();
	if (m_baseSettings.shouldExit)
	{
		SimStatePtr p = std::make_unique<MenuState>();
		ChangeState(se, p);
		return;
	}

	if (activeSceneName != m_baseSettings.activeSceneFilename)
	{
		ScenePtr s = ResourceManager::LoadScene(m_baseSettings.activeSceneFilename);
		simManager = std::make_unique<SimManager>(s);
		BoundaryCallbackFunc func = [&](BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle) {
			int i = 0;
		};

		simManager->GetBoundary()->RegisterCallback(func);
		activeSceneName = m_baseSettings.activeSceneFilename;
	}

	//60 hz timestep
	simManager->Step(1.0f / 60);
}

void MainState::Draw(SimEngine & se)
{
	NVGcontext* vg = se.GetContext();

	//flip y axis
	nvgScale(vg, 1.0f, -1.0f);

	//render simulation objects
	simManager->Render(vg, m_sceneRenderer);

	//unflip y-axis
	nvgScale(vg, 1.0f, -1.0f);

	// draw interface here
	imguiBeginFrame((int)se.GetMouseState().xPos, (int)se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
	uim.DrawBaseUI(m_baseSettings, se.GetWindowState());

	if (m_baseSettings.showStatsPane)
		uim.DrawStatsPane(sm, se.GetWindowState());

	imguiEndFrame();

	guiRenderer.Flush(vg);
	Rectangle r = cam->GetRect();
}

void MainState::HandleEvents(SimEngine & se)
{
	MouseState currState = se.GetMouseState();

	//mouse dragging
	if (currState.rightMouse == GLFW_PRESS && m_dragging == false)
		m_dragging = true;

	if (currState.rightMouse == GLFW_RELEASE && m_dragging == true)
		m_dragging = false;

	if (m_dragging)
	{
		b2Vec2 currOrigin = cam->GetOrigin();
		b2Vec2 increment(0, 0);

		float diffX = currState.xPos - prevMouseState.xPos;
		float diffY = currState.yPos - prevMouseState.yPos;

		if (diffX < 0)
		{
			increment.x = diffX;
		}
		else if(diffX > 0)
		{
			increment.x = diffX;
		}

		if (diffY < 0)
		{
			increment.y = diffY;
		}
		else if (diffY > 0)
		{
			increment.y = diffY;
		}

		cam->SetOrigin(currOrigin + increment);
	}

	prevMouseState = se.GetMouseState();
}

void MainState::OnScroll(double scrollOffset)
{
	float zoom = cam->GetZoom();
	if (scrollOffset < 0)
	{
		cam->SetZoom(zoom / fabs(scrollOffset * 0.5));
	}
	else
	{
		cam->SetZoom(zoom * fabs(scrollOffset * 0.5));
	}
}
