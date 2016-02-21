#include "Simulator.h"

Simulator::Simulator()
	:m_world(new b2World(b2Vec2(0, 0)))
{
	m_world->SetDebugDraw(&g_debugDraw);
}

Simulator::Simulator(WorldPtr world)
{
	m_world = std::move(world);
}

void Simulator::LoadScene(ScenePtr& ptr_scene)
{
	//get scene ptr
	m_currentScene = std::move(ptr_scene);

	BindPhysics();
}

void Simulator::BindPhysics()
{
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.BindPhysics(m_world.get());
	}
}

void Simulator::Step()
{
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.Update(m_currentScene->m_lights);
	}
}

void Simulator::Render()
{
	//render vehicles
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.Render(m_sceneRenderer);
	}

	//render lights
	for (auto &obj : m_currentScene->m_lights)
	{
		obj.Render(m_sceneRenderer);
	}
}
