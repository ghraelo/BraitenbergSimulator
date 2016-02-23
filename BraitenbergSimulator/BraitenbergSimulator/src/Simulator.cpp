#include "Simulator.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

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

			float32 w = float32(g_camera.m_width);
			float32 h = float32(g_camera.m_height);
			float32 ratio = w / h;
			b2Vec2 extents(ratio * 25.0f, 25.0f);
			extents *= g_camera.m_zoom;

			b2Vec2 lower = g_camera.m_center - extents;
			b2Vec2 upper = g_camera.m_center + extents;
			g_debugDraw.DrawPoint(lower, 5, b2Color(1, 0, 1));
			g_debugDraw.DrawPoint(upper, 5, b2Color(1, 0, 1));

			float angle = atan2(dirVec.x, dirVec.y);
			std::cout << "angle: " << angle/M_PI << "pi \n";
			if (angle < -M_PI / 2)
			{
				//use lower y, lower x
				float dx = lower.x - point.x;
				float dy = lower.y - point.y;

				b2Vec2 npt = newVec + std::min(dx, dy) * dirVec;
				g_debugDraw.DrawSegment(point, npt, b2Color(1, 1, 1));
			}
			else if (angle < 0)
			{
				//use upper x, lower y
				float dx = upper.x - point.x;
				float dy = lower.y - point.y;

				b2Vec2 npt = newVec + std::min(dx, dy) * dirVec;
				g_debugDraw.DrawSegment(point, npt, b2Color(1, 1, 1));
			}
			else if (angle < M_PI / 2)
			{
				//use upper x,y
				float dx = upper.x - point.x;
				float dy = upper.x - point.y;
				
				b2Vec2 npt = newVec + std::min(dx, dy) * dirVec;
				g_debugDraw.DrawSegment(point, npt, b2Color(1,1,1));

			}
			else if(angle >= M_PI / 2)
			{
				//use upper y, lower x
				float dx = lower.x - point.x;
				float dy = upper.y - point.y;

				b2Vec2 npt = newVec + std::min(dx, dy) * dirVec;
				g_debugDraw.DrawSegment(point, npt, b2Color(1, 1, 1));
			}
			m_world->RayCast(&lrcc, point, newVec + dirVec);

			if (lrcc.m_collided == true)
			{
				rayCastPoly.push_back(lrcc.m_point);
				//g_debugDraw.DrawSegment(point, lrcc.m_point, col[i]);

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

	g_debugDraw.DrawPoint(avg, 5, b2Color(1, 0, 1));
	//sort
	std::sort(rayCastPoly.begin(), rayCastPoly.end(), 
		[&](b2Vec2 a, b2Vec2 b) {
			float a_angle = atan2(a.y - avg.y, a.x - avg.x);
			float b_angle = atan2(b.y - avg.y, b.x - avg.x);

			return a_angle < b_angle;
			}
		);

	//std::cout << "\r";
}

void Simulator::Render()
{

	g_debugDraw.DrawPoint(b2Vec2(0, 20), 5, b2Color(1, 0, 0));
	if (rayCastPoly.size() > 0)
		g_debugDraw.DrawPolygon(&rayCastPoly[0], rayCastPoly.size(), b2Color(1, 1, 0, 0.5));

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