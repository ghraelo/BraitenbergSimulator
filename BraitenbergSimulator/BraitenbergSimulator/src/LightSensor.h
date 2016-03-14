#pragma once

#include "Box2D\Box2D.h"

#include "LightSource.h"
#include "IRenderable.h"
#include "Raycaster.h"

#include <vector>

//forward declarations
class Renderer;
class Vehicle;

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


//first element is start, second element is end
typedef std::pair<float, float> Interval;

class LightSensor : IRenderable
{
public:
	LightSensor();
	LightSensor(Vehicle* parent);
	LightSensor(Vehicle* parent, sensorInfo info);
	LightSensor(Vehicle* parent,b2Vec2 offset, float aperture, b2Vec2 direction);

	b2Body* GetParentBody();
	b2Vec2 GetPosition();
	void Render(NVGcontext* vg, Renderer& r) override;
	float GetLight(std::vector<LightSource>& lightSources, Rectangle bounds);
	void GetLightBoundary(b2Vec2& lightPos, float lightRadius, std::vector<Interval>& intervals, std::vector<b2Vec2>& rayCastPoly);
	b2Vec2 GetArcEnd(float radius, bool positive);
	sensorInfo GetSensorInfo() const;
	std::vector<Interval> m_intervals;
private:
	Vehicle* m_parent;
	b2Vec2 m_offset;
	float m_aperture_angle;
	b2Vec2 m_direction_vector;

};