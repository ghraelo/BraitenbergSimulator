#include "Simulator.h"

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

Simulator::Simulator()
	:m_world(new b2World(b2Vec2(0, 0)))
{
	m_world->SetDebugDraw(&g_debugDraw);

}

Simulator::Simulator(WorldPtr world)
{
	m_world = std::move(world);

	//init body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	bodyDef.gravityScale = 0.0f;
	bodyDef.angularDamping = 0.5f;
	bodyDef.linearDamping = 0.5f;
	b2Body* body = (*m_world).CreateBody(&bodyDef);
	
	SimObjectInfo* soi = new SimObjectInfo;
	soi->m_obj = new SimObject();
	soi->m_type = "test";
	body->SetUserData(soi);

	//init shape

	b2Vec2 vertices[3];
	vertices[0].Set(-1.0f, 0.0f);
	vertices[1].Set(1.0f, 0.0f);
	vertices[2].Set(0.0f, 3.0f);
	int32 count = 3;

	b2PolygonShape vehicleShape;
	vehicleShape.Set(vertices, count);

	//init fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &vehicleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	//create fixture
	b2Fixture* theFixture = body->CreateFixture(&fixtureDef);
}

void Simulator::LoadScene(ScenePtr& ptr_scene)
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

void Simulator::BindPhysics()
{
	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.BindPhysics(m_world.get());
	}
}

void Simulator::Step()
{
	if(m_tsCount < 30)
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

void Simulator::Sample()
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