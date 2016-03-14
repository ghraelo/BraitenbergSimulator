#pragma once

#include <vector>

#include <Box2D\Box2D.h>

enum RectangleSide { RS_LEFT, RS_RIGHT, RS_TOP, RS_BOTTOM };

class Rectangle
{
public:
	Rectangle();
	Rectangle(b2Vec2 topRight, b2Vec2 bottomLeft);
	std::vector<b2Vec2> GetPoints();
	bool Inside(const b2Vec2& point);
	float GetSidePos(RectangleSide side);
	b2Vec2 m_topLeft;
	b2Vec2 m_bottomLeft;
	b2Vec2 m_topRight;
	b2Vec2 m_bottomRight;
};