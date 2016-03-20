#pragma once

#include <Box2D\Box2D.h>
#include <vector>

#include "Rectangle.h"
#include "IRenderable.h"
class Renderer;

class BoundaryQueryCallback : public b2QueryCallback
{
public:
	std::vector<b2Body*> m_foundBodies;

	bool ReportFixture(b2Fixture* fixture)
	{
		m_foundBodies.push_back(fixture->GetBody());
		return true;
	};
};

class Boundary : public IRenderable
{
public:
	Boundary(b2World* world, b2Vec2 topLeft, float width, float height, float influence);
	void Update();
	void Render(NVGcontext* vg, Renderer& r) override;
	Rectangle GetRect();
	float GetInfluence();
	std::vector<Rectangle> aabbs;
private:
	b2World* m_world;
	float m_influence;
	b2Vec2 m_topLeft;
	float m_width;
	float m_height;
};

typedef std::unique_ptr<Boundary> BoundaryPtr;