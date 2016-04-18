#pragma once
#include "LightSource.h"

#include <vector>

class MovingLightSource : public LightSource
{
public:
	MovingLightSource(b2Vec2 initialPosition, float radius, std::string name, std::vector<b2Vec2> path, float speed);
	void Update() override;
protected:
	std::vector<b2Vec2> m_path;
	int currIndex = 0;
	int prevIndex = 0;
	float m_speed;
};