#include "MainState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"

MainState::MainState()
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

	//create world
	world = std::make_unique<b2World>(b2Vec2(0, 0));

	//load scene and bind physics
	LoadScene(ResourceManager::LoadScene(m_baseSettings.activeSceneFilename));

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->BindPhysics(world.get());
	}

	cam = std::make_unique<Camera>(se.GetWindowState().width,se.GetWindowState().height);
	m_sceneRenderer.SetCamera(cam.get());
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
	world->Step(1.0f/60, 10, 10);
;	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Update(m_currentScene->m_lights,cam->GetRect());
	}

	//wrap sides
}

void MainState::Draw(SimEngine & se)
{
	NVGcontext* vg = se.GetContext();

	// draw interface here
	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
	uim.DrawBaseUI(m_baseSettings, se.GetWindowState());
	imguiEndFrame();

	guiRenderer.Flush(vg);
	nvgTranslate(vg, se.GetWindowState().width/2, se.GetWindowState().height);
	nvgScale(vg, 1.0f, -1.0f);
	//render vehicles

	for (auto &obj : m_currentScene->m_lights)
	{
		obj.Render(vg, m_sceneRenderer);
	}

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Render(vg, m_sceneRenderer);
	}

}

void MainState::LoadScene(ScenePtr & ptr_scene)
{
	m_currentScene = std::move(ptr_scene);
}
