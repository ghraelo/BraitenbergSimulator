#pragma once

#include "SimObject.h"
#include <Box2D\Box2D.h>
#include "SimObjectInfo.h"
#include "IRenderable.h"

//forward declarations
class Renderer;

enum ObstacleType { OT_CIRCLE, OT_SQUARE};

typedef std::unique_ptr<b2Shape> b2ShapePtr;

class Obstacle : public SimObject
{
public:
	Obstacle(b2Vec2 position, ObstacleType type, float size, std::string name);

	void BindPhysics(b2World* world);
	void Render(NVGcontext* vg, Renderer& r) override;
	b2Shape* GetShape();
	ObstacleType GetType();
	b2Body* m_body;
	//TODO: render
protected:
	b2ShapePtr m_shape;

	b2World* theWorld;
	SimObjectInfoPtr m_soiPtr;
	ObstacleType m_type;
	std::string m_name;
};

typedef std::unique_ptr<Obstacle> ObstaclePtr;