#include "MainState.h"

#include "Scene.h"
#include "ResourceManager.h"

MainState::MainState()
{
}

void MainState::Init()
{
	g_debugDraw.Create();
	ScenePtr s = ResourceManager::LoadScene("yaml/testScene.yaml");
	sm.LoadScene(s);
	sm.BindPhysics();
}

void MainState::Cleanup()
{
}

void MainState::Update(SimEngine & se)
{
	sm.Step();
}

void MainState::Draw(SimEngine & se)
{
	sm.Render();
}
