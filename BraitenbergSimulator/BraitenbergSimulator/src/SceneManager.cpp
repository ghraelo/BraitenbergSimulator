#include "SceneManager.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <sstream>
#include <functional>

#include <Windows.h>

#include "MathUtils.h"
#include "LightRayCastCallback.h"
#include "Raycaster.h"
#include "SimObjectInfo.h"

#include "Statistics.h"

#include <fftw3.h>
#include <Box2D\Box2D.h>

SceneManager::SceneManager()
	:m_world(new b2World(b2Vec2(0, 0))),m_dataRecorder(),circ(500)
{
	circ.set_capacity(500);
	for (int i = 0; i < circ.capacity(); i++)
	{
		circ[i] = 0;
	}
	m_world->SetDebugDraw(&g_debugDraw);

}

SceneManager::SceneManager(WorldPtr world)
	:m_dataRecorder(),circ(500)
{
	for (int i = 0; i < circ.capacity(); i++)
	{
		circ.push_back(0);
	}
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
		header.m_cellData.push_back(veh->GetName() + ".x");
		header.m_cellData.push_back(veh->GetName() + ".y");
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
		obj->BindPhysics(m_world.get());

		std::function<float()> angleCallback = [&]() {
			return atan2(obj->m_body->GetWorldVector(b2Vec2(0, 1)).y, obj->m_body->GetWorldVector(b2Vec2(0, 1)).x);
		};

		std::function<float()> distCallback = [&]() {
			return b2Distance(obj->m_body->GetPosition(),m_currentScene->m_lights[0].GetPosition());
		};


		//add statistics watchers
		StatisticPtr s = std::make_unique<FixedWatcher>(obj->GetName() + " - angle",10, 170,
		angleCallback,-M_PI,M_PI);

		StatisticPtr s2 = std::make_unique<Watcher>(obj->GetName() + " - distance", 10, 170,
			distCallback);

		StatisticPtr p1 = std::make_unique<AutocorrelationProcessor>(obj->GetName() + " - acor(distance)", dynamic_cast<Watcher*>(s2.get()));

		StatisticPtr p2 = std::make_unique<AutocorrelationProcessor>(obj->GetName() + " - acor(angle)", dynamic_cast<FixedWatcher*>(s.get()));

		StatisticPtr p3 = std::make_unique<PeriodicityDetectionProcessor>(obj->GetName() + " - dct(angle)", dynamic_cast<Watcher*>(s.get()));

		m_statsManager.AddStat(std::move(s));
		m_statsManager.AddStat(std::move(s2));
		m_statsManager.AddStat(std::move(p1));
		m_statsManager.AddStat(std::move(p2));
		m_statsManager.AddStat(std::move(p3));
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
		obj->Update(m_currentScene->m_lights);
	}
	m_statsManager.Update();
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
		convert << obj->GetPosition().x;
		row.m_cellData.push_back(convert.str());
		convert.str("");
		convert.clear();

		convert << obj->GetPosition().y;
		row.m_cellData.push_back(convert.str());
		convert.str("");
		convert.clear();
	}
	m_dataRecorder.Record(row);
	sampleCount++;
	float dist = b2Distance(m_currentScene->m_vehicles[0]->GetPosition(), m_currentScene->m_lights[0].GetPosition());
	circ.push_back(dist);
}

const StatisticsManager & SceneManager::GetStatsMan() const
{
	return m_statsManager;
}

void SceneManager::Render()
{	
	//render vehicles
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Render(m_sceneRenderer);
}	

	//render lights
	for (auto &obj : m_currentScene->m_lights)
	{
		obj.Render(m_sceneRenderer);
	}
}