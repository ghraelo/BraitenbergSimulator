#include "SceneManager.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "MathUtils.h"
#include "LightRayCastCallback.h"
#include "Raycaster.h"
#include "SimObjectInfo.h"

#include <Windows.h>

SceneManager::SceneManager()
	:m_world(new b2World(b2Vec2(0, 0))),m_dataRecorder()
{
	m_world->SetDebugDraw(&g_debugDraw);

}

SceneManager::SceneManager(WorldPtr world)
	:m_dataRecorder()
{
	m_world = std::move(world);
}

void SceneManager::LoadScene(ScenePtr& ptr_scene)
{
	//get scene ptr
	m_currentScene = std::move(ptr_scene);
	BindPhysics();
	CSVRow header;
	header.m_cellData.push_back("time");
	for (auto& veh : m_currentScene->m_vehicles)
	{
		header.m_cellData.push_back(veh.GetName() + ".x");
		header.m_cellData.push_back(veh.GetName() + ".y");
	}
	m_dataRecorder.BeginFile(header);
	char buffer[100];
	GetCurrentDirectory(100, buffer);
	printf("current directory: %s", buffer);
}

void SceneManager::BindPhysics()
{
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.BindPhysics(m_world.get());
	}
}

void SceneManager::Step()
{
	if(m_tsCount < 5)
		m_tsCount++;
	else
	{
		m_tsCount = 0;
		Sample();
	}
	//RayCast(b2Vec2(0, 20));

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.Update(m_currentScene->m_lights);
	}
}

void SceneManager::Sample()
{
	CSVRow row;

	std::ostringstream convert;
	convert << sampleCount;
	row.m_cellData.push_back(convert.str());
	convert.str("");
	convert.clear();
	for (auto &obj : m_currentScene->m_vehicles)
	{
		convert << obj.GetPosition().x;
		row.m_cellData.push_back(convert.str());
		convert.str("");
		convert.clear();

		convert << obj.GetPosition().y;
		row.m_cellData.push_back(convert.str());
		convert.str("");
		convert.clear();
	}
	m_dataRecorder.Record(row);

	sampleCount++;
}


void SceneManager::Render()
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