#include "SimManager.h"

SimManager::SimManager()
{
}

SimManager::SimManager(ScenePtr& initialScene)
{
	//load scene
	LoadScene(initialScene);

	//create world
	b2Vec2 origin = b2Vec2_zero;
	m_world = std::make_unique<b2World>(origin);

	//bind vehicle physics
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->BindPhysics(m_world.get());
	}

	//create test boundary
	m_boundary = std::make_unique<Boundary>(m_world.get(), b2Vec2(-50.0f, 50.0f), 100.0f, 100.0f, 20.0f);

}

Scene* SimManager::GetCurrentScene()
{
	return m_currentScene.get();
}

void SimManager::LoadScene(ScenePtr & scene)
{
	m_currentScene = std::move(scene);
}

void SimManager::Step(float timeStep)
{
	m_world->Step(timeStep, 8, 3);

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Update(m_currentScene->m_lights, m_boundary->GetRect());
	}

	m_boundary->Update();
}

void SimManager::Render(NVGcontext * vg, Renderer & renderer)
{
	for (auto &obj : m_currentScene->m_lights)
	{
		obj.Render(vg,renderer);
	}

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Render(vg, renderer);
	}

	m_boundary->Render(vg, renderer);
}
