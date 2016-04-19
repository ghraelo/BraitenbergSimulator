#pragma once

#include <Box2D\Box2D.h>

#include "LightSensor.h"
#include "IRenderable.h"
#include "SimObject.h"
#include "ControlStrategy.h"
#include "SimObjectInfo.h"

#include <string>
#include <map>

//forward declarations
class Renderer;

class Vehicle : public SimObject
{
public:
	Vehicle();
	Vehicle(b2Vec2 position, sensorInfo leftInfo, sensorInfo rightInfo, ControlStrategyPtr& strategy, std::string name);
	Vehicle(b2Vec2 position, sensorInfo leftInfo, sensorInfo rightInfo, ControlStrategyPtr& strategy,std::string name, unsigned int seedLeft, unsigned int seedRight);
	//Vehicle(b2World* world, VehicleDef vehicleDef);
	~Vehicle();
	void BindPhysics(b2World* world);
	void LeftForce(float magnitude);
	void RightForce(float magnitude);
	void Update() override;
	void Update(std::vector<LightSourcePtr>& ls, Rectangle bounds);
	b2Vec2 GetPosition() override;
	void SetPosition(b2Vec2 pos);
	void SetAngle(float ang);
	b2Vec2 GetCOM();
	b2Body* m_body;
	LightSensor leftSensor;
	LightSensor rightSensor;
	void Render(NVGcontext* vg, Renderer& r) override;
	void EnableControl();
	void DisableControl();
	b2World* GetWorld();
	bool ControlStatus();
	std::map<std::string, float> GetInternalData();
	
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