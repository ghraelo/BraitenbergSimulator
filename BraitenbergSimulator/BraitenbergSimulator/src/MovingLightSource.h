#pragma once
#include "LightSource.h"

#include <vector>

class MovingLightSource : public LightSource
{
public:
	MovingLightSource(float radius, std::string name, std::vector<b2Vec2> path, float speed);
	void Update() override;
protected:
	std::vector<b2Vec2> m_path;
	int targetIndex = 0;
	float m_speed; //speed in units/s
	bool WithinRange(b2Vec2 src, b2Vec2 target);
};