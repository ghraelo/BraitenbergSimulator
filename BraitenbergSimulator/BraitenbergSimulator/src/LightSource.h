#pragma once

#include "Box2D\Box2D.h"
#include "SimObject.h"
#include <functional>


class LightSource : SimObject
{
public:
	b2Body* m_body;
	LightSource();
	LightSource(b2Vec2 position, float radius);
	float GetRadius();
protected:
	float m_radius;
};