#include "Simulator.h"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>

#include "MathUtils.h"
#include "LightRayCastCallback.h"


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
	RayCast(b2Vec2(0, 20));

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj.Update(m_currentScene->m_lights);
	}
}

void Simulator::RayCast(b2Vec2 point)
{
	rayCastPoly.clear();
	//get all points
	std::vector<b2Vec2> vertices;

	b2Body* bList = m_world->GetBodyList();

	while (bList != NULL)
	{

		//get all fixtures
		b2Fixture* fList = bList->GetFixtureList();

		while (fList != NULL)
		{
			switch (fList->GetType())
			{
			case b2Shape::e_circle:
			{
				b2CircleShape* circle = (b2CircleShape*)fList->GetShape();
				/* Do stuff with a circle shape */
			}
			break;

			case b2Shape::e_polygon:
			{
				b2PolygonShape* poly = (b2PolygonShape*)fList->GetShape();
				/* Do stuff with a polygon shape */
				std::vector<b2Vec2> polyVertices(poly->m_vertices,poly->m_vertices + poly->GetVertexCount());
				for (auto& vert : polyVertices)
				{
					vert = bList->GetWorldPoint(vert);
				}
				vertices.insert(vertices.end(), polyVertices.begin(),polyVertices.end());
			}
			break;
			}
			fList = fList->GetNext();
		}

		bList = bList->GetNext();
	}
	//std::cout << "vertices: ";

	vertices.push_back(g_camera.GetCorner(CC_TOP_RIGHT));
	vertices.push_back(g_camera.GetCorner(CC_TOP_LEFT));
	vertices.push_back(g_camera.GetCorner(CC_BOTTOM_RIGHT));
	vertices.push_back(g_camera.GetCorner(CC_BOTTOM_LEFT));

	for (auto& vec : vertices)
	{
		float rot[3] = { 0, 0.001, -0.001 };
		b2Color col[3] = { b2Color(1, 0, 0) , b2Color(0, 1, 0) , b2Color(0, 0, 1) };

		for (int i = 0; i < 3; i++)
		{
			//cast ray at vertex
			LightRayCastCallback lrcc;
			b2Vec2 newVec = MathUtils::RotateAroundPoint(vec, point, rot[i]);
			b2Vec2 dirVec = newVec - point;
			dirVec.Normalize();

			float dx = 0, dy = 0;

			if (dirVec.x > 0 && dirVec.y > 0) //top right
			{
				dx = g_camera.GetCorner(CC_TOP_RIGHT).x - point.x;
				dy = g_camera.GetCorner(CC_TOP_RIGHT).y - point.y;
			}
			else if (dirVec.x > 0 && dirVec.y < 0) //bottom right
			{
				dx = g_camera.GetCorner(CC_BOTTOM_RIGHT).x - point.x;
				dy = g_camera.GetCorner(CC_BOTTOM_RIGHT).y - point.y;
			}
			else if (dirVec.x < 0 && dirVec.y > 0) //top left
			{
				dx = g_camera.GetCorner(CC_TOP_LEFT).x - point.x;
				dy = g_camera.GetCorner(CC_TOP_LEFT).y - point.y;
			}
			else if (dirVec.x < 0 && dirVec.y < 0) //bottom left
			{
				dx = g_camera.GetCorner(CC_BOTTOM_LEFT).x - point.x;
				dy = g_camera.GetCorner(CC_BOTTOM_LEFT).y - point.y;
			}

			dx = fabs(dx);
			dy = fabs(dy);
			float sf = 0; //scale factor

			if (dx < dy)
			{
				sf = dx / dirVec.x;
			}
			else
			{
				sf = dy / dirVec.y;
			}

			b2Vec2 npt = point +  fabs(sf) * dirVec;
			m_world->RayCast(&lrcc, point, newVec + dirVec);

			if (lrcc.m_collided == true)
			{
				rayCastPoly.push_back(lrcc.m_point);
				g_debugDraw.DrawSegment(point, lrcc.m_point, b2Color(1,0,0));

			}
			else
			{
				rayCastPoly.push_back(npt);
				g_debugDraw.DrawSegment(point, npt, b2Color(0,1,0));
			}
		}
		//std::cout << std::setprecision(2);
		//std::cout << "(" << vec.x << ", " << vec.y << "), ";
	}

	b2Vec2 avg(0,0);
	for (auto& pt : rayCastPoly)
	{
		avg.x = avg.x + pt.x;
		avg.y = avg.y + pt.y;
	}

	avg.x = avg.x / rayCastPoly.size();
	avg.y = avg.y / rayCastPoly.size();
	avg = point;
	//g_debugDraw.DrawPoint(avg, 5, b2Color(1, 0, 1));
	//sort
	std::sort(rayCastPoly.begin(), rayCastPoly.end(), 
		[&](b2Vec2 a, b2Vec2 b) {

			float a_angle = atan2(a.y - avg.y, a.x - avg.x);
			float b_angle = atan2(b.y - avg.y, b.x - avg.x);
			if (a_angle == b_angle)
				return b2Distance(a, avg) > b2Distance(b, avg);
			else
				return a_angle > b_angle;
			}
		);

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
		g_debugDraw.DrawConcavePolygon(rayCastPoly, b2Color(1, 1, 0.5, 1));

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