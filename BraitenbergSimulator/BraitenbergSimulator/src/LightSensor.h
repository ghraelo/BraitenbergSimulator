#pragma once

#include "Box2D\Box2D.h"
#include "DebugDraw.h"
#include "LightSensor.h"

struct relLightPos
{
	float m_point1;
	float m_point2;
};

struct sensorInfo
{
	b2Vec2 m_offset;
	float m_aperture;
	b2Vec2 m_direction;
};

class LightSensor
{
public:
	LightSensor();
	LightSensor(b2Body* parent, sensorInfo info);
	LightSensor(b2Body* parent);
	LightSensor(b2Body* parent,b2Vec2 offset, float aperture, b2Vec2 direction);
	b2Vec2 GetWorldPosition();

	void GetLightBoundary(b2Vec2& lightPos, float lightRadius, relLightPos& relativeLightBoundary);
	b2Vec2 GetArcEnd(float radius, bool positive);
protected:
	void GetIntersection(b2Vec2& lightPos, float lightRadius, b2Vec2& right_intersection, b2Vec2& left_intersection);
	b2Vec2 GetPointProjection(b2Vec2& lineA, b2Vec2& lineB, b2Vec2& point);

private:
	b2Body* m_parent;
	b2Vec2 m_offset;
	float m_aperture_angle;
	b2Vec2 m_direction_vector;
};