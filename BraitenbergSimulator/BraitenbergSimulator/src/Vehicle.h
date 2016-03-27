#pragma once

#include <Box2D\Box2D.h>

#include "LightSensor.h"
#include "IRenderable.h"
#include "SimObject.h"
#include "ControlStrategy.h"
#include "SimObjectInfo.h"
#include <string>

//forward declarations
class Renderer;

class Vehicle : public SimObject
{
public:
	Vehicle();
	Vehicle(sensorInfo leftInfo, sensorInfo rightInfo, ControlStrategyPtr& strategy,std::string name);
	//Vehicle(b2World* world, VehicleDef vehicleDef);
	~Vehicle();
	void BindPhysics(b2World* world);
	void LeftForce(float magnitude);
	void RightForce(float magnitude);
	void Update() override;
	void Update(std::vector<LightSource> ls, Rectangle bounds);
	b2Vec2 GetPosition() override;
	void SetPosition(b2Vec2 pos);
	b2Vec2 GetCOM();
	std::string GetName();
	b2Body* m_body;
	LightSensor leftSensor;
	LightSensor rightSensor;
	void Render(NVGcontext* vg, Renderer& r) override;
	void EnableControl();
	void DisableControl();
	bool ControlStatus();
	
private:
	std::string m_name;
	ControlStrategyPtr m_ctrl_strat;
	b2BodyDef bodyDef;
	b2PolygonShape vehicleShape;
	b2FixtureDef fixtureDef;
	b2World* theWorld;
	bool m_physicsBound = false;
	bool m_controllerEnabled = true;
	void SetUserData();
	SimObjectInfoPtr m_soiPtr;
};

typedef std::unique_ptr<Vehicle> VehiclePtr;