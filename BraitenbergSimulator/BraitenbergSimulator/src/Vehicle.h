#pragma once

#include "Box2D\Box2D.h"
#include "Box2D\Common\b2Math.h"
#include "LightSensor.h"
#include "IRenderable.h"
#include "SimObject.h"

//forward declarations
class Renderer;

class Vehicle : SimObject, IRenderable
{
public:
	Vehicle(b2World* world);
	Vehicle(b2World* world, sensorInfo leftInfo, sensorInfo rightInfo);
	//Vehicle(b2World* world, VehicleDef vehicleDef);
	~Vehicle();
	void Initialise();
	void LeftForce(float magnitude);
	void RightForce(float magnitude);
	void Update() override;
	b2Body* m_body;
	LightSensor leftSensor;
	LightSensor rightSensor;
	void Render(Renderer& r) override;
private:
	b2BodyDef bodyDef;
	b2PolygonShape vehicleShape;
	b2FixtureDef fixtureDef;
	b2World* theWorld;
};

typedef std::shared_ptr<Vehicle> VehiclePtr;