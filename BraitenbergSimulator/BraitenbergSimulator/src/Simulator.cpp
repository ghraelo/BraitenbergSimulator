#include "Simulator.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>

#include "MathUtils.h"
#include "LightRayCastCallback.h"
#include "Raycaster.h"
#include "SimObjectInfo.h"

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
	//RayCast(b2Vec2(0, 20));

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.Update(m_currentScene->m_lights);
	}
}

void Simulator::RayCast(b2Vec2 point)
{
	Raycaster r(m_world.get(), point, Rectangle(g_camera.GetCorner(CC_TOP_RIGHT), g_camera.GetCorner(CC_BOTTOM_LEFT)));
	r.Cast(1.1f, 1.0f);
	if (r.GetRayCastPoly().size() > 0)
	{
		g_debugDraw.DrawConcavePolygon(r.GetRayCastPoly(), b2Color(1, 0, 1));
		g_debugDraw.DrawPolygon(&r.GetRayCastPoly()[0], r.GetRayCastPoly().size(), b2Color(1, 0, 1));
	}

	b2Vec2 angle1Dir(cos(1.0f), sin(1.0f));
	angle1Dir.Normalize();
	b2Vec2 angle2Dir(cos(1.1f), sin(1.1f));
	angle2Dir.Normalize();

	g_debugDraw.DrawSegment(point, point + 25 * angle1Dir, b2Color(1, 0, 0));
	g_debugDraw.DrawSegment(point, point + 25 * angle2Dir, b2Color(0, 0, 1));
}

void Simulator::Render()
{

	g_debugDraw.DrawPoint(b2Vec2(0, 20), 5, b2Color(1, 0, 0));
	/*
	std::vector<b2Vec2> scaled = rayCastPoly;
	std::vector<b2Vec2> rect;
	rect.push_back((0.1 *g_camera.GetCorner(CC_TOP_LEFT)));
	rect.push_back((0.1 *g_camera.GetCorner(CC_TOP_RIGHT)));
	rect.push_back((0.1 *g_camera.GetCorner(CC_BOTTOM_RIGHT)));
	rect.push_back((0.1 *g_camera.GetCorner(CC_BOTTOM_LEFT)));
	g_debugDraw.DrawPolygon(&rect[0], rect.size(), b2Color(0, 1, 0, 0.5));
	for (auto& sc : scaled)
	{
		sc = 0.1 * sc;
	}

	if (scaled.size() > 0)
	{

		g_debugDraw.DrawPolygon(&scaled[0], scaled.size(), b2Color(0, 1, 1, 1));
	}*/

	if (rayCastPoly.size() > 0)
		g_debugDraw.DrawConcavePolygon(rayCastPoly, b2Color(1, 1, 1, 1));

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