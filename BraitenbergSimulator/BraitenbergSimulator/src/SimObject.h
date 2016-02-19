#pragma once

#include <Box2D\Common\b2Math.h>

class SimObject
{
public:
	SimObject();
	SimObject(b2Vec2 position);
	virtual b2Vec2 GetPosition();
	virtual void Update() = 0;
protected:
	b2Vec2 m_position;
};