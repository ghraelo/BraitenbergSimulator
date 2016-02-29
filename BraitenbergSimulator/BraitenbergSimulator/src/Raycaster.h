#pragma once

#include <vector>

#include <Box2D\Box2D.h>
#include "Rectangle.h"

class Raycaster
{
public:
	Raycaster();
	Raycaster(b2World* world, b2Vec2 castPoint, Rectangle bounds);
	void Cast();
	void Cast(float angle1, float angle2);
	void SetCastPoint(b2Vec2 castPoint);
	void AddIgnoreBody(b2Body* body);
	b2Vec2 GetCastPoint();
	std::vector<b2Vec2> GetRayCastPoly();
	void SetBounds(Rectangle newBounds);
	Rectangle GetBounds();
private:
	std::vector<b2Vec2> GetVertices();
	std::vector<b2Vec2> DoRays(std::vector<b2Vec2> vertices);
	std::vector<b2Vec2> m_rayCastPolygon;
	std::vector<b2Body*> m_ignoreBodies;
	b2Vec2 m_castPoint;
	b2World* m_world;
	Rectangle m_bounds;
};