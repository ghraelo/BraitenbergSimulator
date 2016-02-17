#pragma once

#include "Box2D\Box2D.h"
#include <functional>


class LightSource
{
public:
	b2Body* m_body;
	LightSource(b2World* world, b2Vec2 position);
	~LightSource();
	float getIntensity();
	b2Vec2 get_position();
	void set_position(b2Vec2 position);
private:
	float light_intensity;
	b2Vec2 light_position;
	
	b2CircleShape light_boundary;
	b2World* m_world;

};