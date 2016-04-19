#pragma once

#include "Box2D\Box2D.h"

#include "LightSource.h"
#include "IRenderable.h"
#include "Raycaster.h"

#include <vector>
#include <random>

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

typedef std::unique_ptr<std::mt19937> mt19937Ptr;
typedef std::unique_ptr<std::normal_distribution<double>> normal_distributionPtr;

class LightSensor : IRenderable
{
public:
	LightSensor();
	LightSensor(Vehicle* parent);
	LightSensor(Vehicle* parent, sensorInfo info);
	LightSensor(Vehicle* parent, sensorInfo info, float randFraction, unsigned int seed);
	LightSensor(Vehicle* parent,b2Vec2 offset, float aperture, b2Vec2 direction);

	b2Body* GetParentBody();
	b2Vec2 GetPosition();
	void Render(NVGcontext* vg, Renderer& r) override;
	float GetLight(std::vector<LightSourcePtr>& lightSources, Rectangle bounds);
	void GetLightBoundary(b2Vec2& lightPos, float lightRadius, std::vector<Interval>& intervals, std::vector<b2Vec2>& rayCastPoly);
	b2Vec2 GetArcEnd(float radius, bool positive);
	sensorInfo GetSensorInfo() const;
	std::vector<Interval> m_intervals;
	const std::vector<b2Vec2>& GetRayCastPolygon();
private:
	Vehicle* m_parent;
	b2Vec2 m_offset;
	float m_aperture_angle;
	b2Vec2 m_direction_vector;
	std::vector<b2Vec2> m_rayCastPoly;
	bool random = false;
	mt19937Ptr mt;
	normal_distributionPtr dist;
};