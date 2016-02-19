#pragma once

#include "Box2D\Box2D.h"
#include "SimObject.h"
#include "IRenderable.h"
#include <functional>

//forward declarations
class Renderer;

class LightSource : public SimObject, IRenderable
{
public:
	b2Body* m_body;
	LightSource();
	LightSource(b2Vec2 position, float radius);
	float GetRadius();
	void Render(Renderer& r) override;
	void Update() override;
protected:
	float m_radius;
};