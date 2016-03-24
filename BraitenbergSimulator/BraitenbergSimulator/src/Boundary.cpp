#include "Boundary.h"
#include "Renderer.h"

Boundary::Boundary(b2World * world, b2Vec2 topLeft, float width, float height, float influence)
	:m_world(world), m_topLeft(topLeft), m_width(width), m_height(height), m_influence(influence)
{

}

void Boundary::Update()
{
	aabbs.clear();
	//get objects within influence region using AABB querying
	b2AABB theAABB;
	BoundaryQueryCallback theQuery;

	//top
	b2Vec2 a = m_topLeft;
	b2Vec2 b = m_topLeft + b2Vec2(m_width, -m_influence);

	theAABB.lowerBound = b2Vec2(fmin(a.x,b.x),fmin(a.y,b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	aabbs.push_back(Rectangle(theAABB.lowerBound, theAABB.upperBound));

	m_world->QueryAABB(&theQuery, theAABB);

	for (auto& body : theQuery.m_foundBodies)
	{
		//repel body
		b2Vec2 force(0, theAABB.upperBound.y - theAABB.lowerBound.y);
		b2Vec2 point = body->GetWorldPoint(b2Vec2(0, 2.0f));
		body->ApplyForce(0.05 * force, point, true);
		printf("APPLYING FORCE TOP\n");
	}
	theQuery.m_foundBodies.clear();

	//bottom
	a = m_topLeft + b2Vec2(0,-m_height);
	b = m_topLeft + b2Vec2(m_width, - m_height + m_influence);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	aabbs.push_back(Rectangle(theAABB.lowerBound, theAABB.upperBound));

	m_world->QueryAABB(&theQuery, theAABB);

	for (auto& body : theQuery.m_foundBodies)
	{
		//repel body
		b2Vec2 force(0, theAABB.upperBound.y - theAABB.lowerBound.y);
		b2Vec2 point = body->GetWorldPoint(b2Vec2(0, 2.0f));
		body->ApplyForce(0.05 * force, point, true);
		printf("APPLYING FORCE BOTTOM\n");
	}
	theQuery.m_foundBodies.clear();

	//left
	a = m_topLeft;
	b = m_topLeft + b2Vec2(m_influence, -m_height);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	aabbs.push_back(Rectangle(theAABB.lowerBound, theAABB.upperBound));

	m_world->QueryAABB(&theQuery, theAABB);

	for (auto& body : theQuery.m_foundBodies)
	{
		//repel body
		b2Vec2 force(theAABB.upperBound.x - theAABB.lowerBound.x,0);
		b2Vec2 point = body->GetWorldPoint(b2Vec2(0, 2.0f));
		body->ApplyForce(0.05*force, point, true);
		printf("APPLYING FORCE LEFT\n");
	}
	theQuery.m_foundBodies.clear();

	//right
	a = m_topLeft + b2Vec2(m_width,0);
	b = m_topLeft + b2Vec2(m_width - m_influence, -m_height);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	aabbs.push_back(Rectangle(theAABB.lowerBound, theAABB.upperBound));

	m_world->QueryAABB(&theQuery, theAABB);

	for (auto& body : theQuery.m_foundBodies)
	{
		//repel body
		b2Vec2 force(theAABB.lowerBound.x - theAABB.upperBound.x,0);
		b2Vec2 point = body->GetWorldPoint(b2Vec2(0, 2.0f));
		body->ApplyForce(0.05 * force, point, true);
		printf("APPLYING FORCE RIGHT\n");
	}
	theQuery.m_foundBodies.clear();

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
