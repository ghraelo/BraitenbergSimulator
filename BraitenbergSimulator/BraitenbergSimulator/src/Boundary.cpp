#include "Boundary.h"
#include "Renderer.h"
#include "SimObjectInfo.h"

Boundary::Boundary(b2World * world, b2Vec2 topLeft, float width, float height, float influence)
	:m_world(world), m_topLeft(topLeft), m_width(width), m_height(height), m_influence(influence)
{
	b2AABB theAABB;
	BoundaryQueryCallback theQuery;

	//top
	b2Vec2 a = m_topLeft;
	b2Vec2 b = m_topLeft + b2Vec2(m_width, m_influence);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	b2Vec2 force(0, theAABB.upperBound.y - theAABB.lowerBound.y);
	force.Normalize();
	AreaForcePair afp(theAABB, 0.95 * -m_height * force);

	aabbs.push_back(afp);

	//bottom
	a = m_topLeft + b2Vec2(0, -m_height);
	b = m_topLeft + b2Vec2(m_width, -m_height - m_influence);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	
	force = b2Vec2(0, theAABB.upperBound.y - theAABB.lowerBound.y);
	force.Normalize();
	afp = AreaForcePair(theAABB, 0.95 * m_height * force);

	aabbs.push_back(afp);




	//left
	a = m_topLeft;
	b = m_topLeft + b2Vec2(-m_influence, -m_height);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	force = b2Vec2(theAABB.upperBound.x - theAABB.lowerBound.x, 0);
	force.Normalize();
	afp = AreaForcePair(theAABB, 0.95 * m_width * force);

	aabbs.push_back(afp);


	//right
	a = m_topLeft + b2Vec2(m_width, 0);
	b = m_topLeft + b2Vec2(m_width + m_influence, -m_height);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	force = b2Vec2(theAABB.lowerBound.x - theAABB.upperBound.x, 0);
	force.Normalize();
	afp = AreaForcePair(theAABB, 0.95 * m_width * force);

	aabbs.push_back(afp);

}

void Boundary::Update()
{
	m_isColliding = false;
	for (auto& afp : aabbs)
	{
		BoundaryQueryCallback theQuery;
		m_world->QueryAABB(&theQuery, afp.first);

		if (!theQuery.m_foundBodies.empty())
		{
			m_isColliding = true;
			for (auto& body : theQuery.m_foundBodies)
			{
				void* info = body->GetUserData();
				SimObjectInfo* s = (SimObjectInfo*)info;

				if (s->m_type == "Vehicle")
				{
					Vehicle* v = (Vehicle*)s->m_obj;
					//printf("current position: (%f,%f)\nafp.second: (%f,%f)\n", v->GetPosition().x, v->GetPosition().y, afp.second.x,afp.second.y);
					v->SetPosition(v->GetPosition() + afp.second);
				}
				//repel body
				//b2Vec2 point = body->GetWorldPoint(b2Vec2(0, 2.0f));
				//body->ApplyForce(afp.second, point, true);
			}

			theQuery.m_foundBodies.clear();
		}
	}
}

void Boundary::Render(NVGcontext * vg, Renderer & r)
{
	r.RenderBoundary(vg, *this);
}

Rectangle Boundary::GetRect()
{
	return Rectangle(m_topLeft + b2Vec2(m_width,0),m_topLeft - b2Vec2(0,m_height));
}

float Boundary::GetInfluence()
{
	return m_influence;
}

bool Boundary::IsColliding()
{
	return m_isColliding;
}
