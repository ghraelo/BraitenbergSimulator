#pragma once

#include <Box2D\Box2D.h>
#include <vector>
#include <functional>

#include "Rectangle.h"
#include "IRenderable.h"
#include "Vehicle.h"

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

enum BoundaryType { BT_LEFT, BT_RIGHT, BT_TOP, BT_BOTTOM };

struct BoundaryData
{
	b2AABB m_aabb;
	b2Vec2 m_force;
	BoundaryType m_type;
	b2Vec2 m_midPt;
};

typedef std::function<void(BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle)> BoundaryCallbackFunc;

class Boundary : public IRenderable
{
public:
	Boundary(b2World* world, b2Vec2 topLeft, float width, float height, float influence);
	void RegisterCallback(BoundaryCallbackFunc func);
	virtual void Update(double time) = 0;
	virtual void Render(NVGcontext* vg, Renderer& r) = 0;
	Rectangle GetRect();
	float GetInfluence();	
	bool IsColliding();
protected:
	void OnCollision(BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle);
	b2World* m_world;
	float m_influence;
	b2Vec2 m_topLeft;
	float m_width;
	float m_height;
	bool m_isColliding = false;
	BoundaryCallbackFunc m_bcf;
	std::vector<BoundaryData> edges;
};

typedef std::unique_ptr<Boundary> BoundaryPtr;