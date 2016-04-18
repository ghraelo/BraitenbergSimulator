#include "Boundary.h"
#include "Renderer.h"
#include "SimObjectInfo.h"

Boundary::Boundary(b2World * world, b2Vec2 topLeft, float width, float height, float influence)
	:m_world(world), m_topLeft(topLeft), m_width(width), m_height(height), m_influence(influence)
{

}

void Boundary::RegisterCallback(BoundaryCallbackFunc func)
{
	m_bcf = func;
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

void Boundary::OnCollision(BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle)
{
	m_bcf(boundary, coords, time, vehicle);
}
