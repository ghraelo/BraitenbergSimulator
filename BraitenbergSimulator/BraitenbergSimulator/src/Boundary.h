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
};

typedef std::function<void(BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle)> BoundaryCallbackFunc;

class Boundary : public IRenderable
{
public:
	Boundary(b2World* world, b2Vec2 topLeft, float width, float height, float influence);
	void RegisterCallback(BoundaryCallbackFunc func);
	void Update(double time);
	void Render(NVGcontext* vg, Renderer& r) override;
	Rectangle GetRect();
	float GetInfluence();
	std::vector<BoundaryData> edges;
	bool IsColliding();
private:
	void OnCollision(BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle);
	b2World* m_world;
	float m_influence;
	b2Vec2 m_topLeft;
	float m_width;
	float m_height;
	bool m_isColliding = false;
	BoundaryCallbackFunc m_bcf;
};

typedef std::unique_ptr<Boundary> BoundaryPtr;