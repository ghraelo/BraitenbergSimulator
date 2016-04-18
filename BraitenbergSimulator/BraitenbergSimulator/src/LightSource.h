#pragma once

#include "Box2D\Box2D.h"
#include "SimObject.h"
#include "IRenderable.h"
#include <functional>

//forward declarations
class Renderer;

class LightSource : public SimObject
{
public:
	//b2Body* m_body;
	LightSource();
	LightSource(b2Vec2 position, float radius, std::string name);
	float GetRadius();
	void Render(NVGcontext* vg, Renderer& r) override;
	virtual void Update() override;
protected:
	float m_radius;
};

typedef std::unique_ptr<LightSource> LightSourcePtr;