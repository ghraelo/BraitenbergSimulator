#pragma once

#include <Box2D\Common\b2Math.h>
#include <string>

#include "IRenderable.h"
class Renderer;

class SimObject : public IRenderable
{
public:
	SimObject();
	SimObject(b2Vec2 position, std::string name);
	virtual b2Vec2 GetPosition();
	virtual void Update();
	virtual void Render(NVGcontext* vg, Renderer& r);
	virtual std::string GetName();
protected:
	b2Vec2 m_position;
	std::string m_name;
};