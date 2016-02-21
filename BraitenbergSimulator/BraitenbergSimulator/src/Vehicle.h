#pragma once

#include "Box2D\Box2D.h"
#include "Box2D\Common\b2Math.h"
#include "LightSensor.h"
#include "IRenderable.h"
#include "SimObject.h"

//forward declarations
class Renderer;

class Vehicle : public SimObject
{
public:
	Vehicle();
	Vehicle(sensorInfo leftInfo, sensorInfo rightInfo);
	Vehicle(Vehicle const& other);
	//Vehicle(b2World* world, VehicleDef vehicleDef);
	~Vehicle();
	void BindPhysics(b2World* world);
	void LeftForce(float magnitude);
	void RightForce(float magnitude);
	void Update() override;
	void Update(std::vector<LightSource> ls);
	b2Vec2 GetPosition() override;
	b2Vec2 GetCOM();
	b2Body* m_body;
	LightSensor leftSensor;
	LightSensor rightSensor;
	void Render(Renderer& r) override;
private:
	b2BodyDef bodyDef;
	b2PolygonShape vehicleShape;
	b2FixtureDef fixtureDef;
	b2World* theWorld;
	bool m_physicsBound = false;
	void SetUserData();
};

typedef std::shared_ptr<Vehicle> VehiclePtr;