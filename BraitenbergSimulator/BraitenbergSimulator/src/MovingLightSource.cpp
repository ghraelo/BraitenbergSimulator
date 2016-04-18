#include "stdafx.h"
#include "MovingLightSource.h"

MovingLightSource::MovingLightSource(b2Vec2 initialPosition, float radius, std::string name, std::vector<b2Vec2> path, float speed)
	:LightSource(initialPosition,radius,name), m_path(path), m_speed(speed)
{
}

void MovingLightSource::Update()
{
	//TODO: move from one path point to another
}
