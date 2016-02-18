#pragma once

#include <Box2D\Common\b2Math.h>

class SimObject
{
public:
	SimObject();
	SimObject(b2Vec2 position);
	b2Vec2 GetPosition();
protected:
	b2Vec2 m_position;
};