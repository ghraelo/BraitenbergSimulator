#include "Vehicle.h"
#include "Renderer.h"
#include "SimObjectInfo.h"

Vehicle::Vehicle()
	:SimObject()
{
}

Vehicle::Vehicle(sensorInfo leftInfo, sensorInfo rightInfo, ControlStrategyPtr& strategy, std::string name)
	:SimObject(), leftSensor(this, leftInfo), rightSensor(this, rightInfo), m_ctrl_strat(std::move(strategy)), m_name(name)
{
}

Vehicle::~Vehicle()
{
	if (m_physicsBound)
	{
		delete (SimObjectInfo*)m_body->GetUserData();
		//theWorld->DestroyBody(m_body);
	}
}

b2Vec2 Vehicle::GetPosition()
{
	m_position = m_body->GetPosition();
	return m_body->GetPosition();
}

b2Vec2 Vehicle::GetCOM()
{
	return m_body->GetWorldCenter();
}

std::string Vehicle::GetName()
{
	return m_name;
}

void Vehicle::SetUserData()
{
	//user data
	SimObjectInfo* soi = new SimObjectInfo;
	soi->m_obj = this;
	soi->m_type = "Vehicle";
	m_body->SetUserData(soi);
}

void Vehicle::BindPhysics(b2World* world)
{
	theWorld = world;
	//init body
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	bodyDef.gravityScale = 0.0f;
	bodyDef.angularDamping = 0.5f;
	bodyDef.linearDamping = 0.5f;
	m_body = (*theWorld).CreateBody(&bodyDef);

	SetUserData();

	//init shape

	b2Vec2 vertices[3];
	vertices[0].Set(-1.0f, 0.0f);
	vertices[1].Set(1.0f, 0.0f);
	vertices[2].Set(0.0f, 3.0f);
	int32 count = 3;

	vehicleShape.Set(vertices,count);

	//init fixture

	fixtureDef.shape = &vehicleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	//create fixture
	b2Fixture* theFixture = m_body->CreateFixture(&fixtureDef); 
}

void Vehicle::LeftForce(float magnitude)
{
	b2Vec2 left(-1.0f, 0.0f);


	b2Vec2 forceDirection = m_body->GetWorldVector(b2Vec2(0, 1));
	forceDirection = magnitude * forceDirection;

	b2Vec2 leftCorner = m_body->GetWorldPoint(left);
	m_body->ApplyForce(forceDirection, leftCorner, true);
}

void Vehicle::RightForce(float magnitude)
{
	b2Vec2 right(1.0f, 0.0f);


	b2Vec2 forceDirection = m_body->GetWorldVector(b2Vec2(0, 1));
	forceDirection = magnitude * forceDirection;

	b2Vec2 rightCorner = m_body->GetWorldPoint(right);
	m_body->ApplyForce(forceDirection, rightCorner, true);
}

void Vehicle::Update()
{

}

void Vehicle::Update(std::vector<LightSource> ls, Rectangle bounds)
{
	float leftLight = leftSensor.GetLight(ls, bounds);
	float rightLight = rightSensor.GetLight(ls, bounds);
	//printf("leftlight: %f, rightlight: %f\n", leftLight, rightLight);
	float leftForce = 0;
	float rightForce = 0;

	if (m_controllerEnabled)
	{
		m_ctrl_strat->SetInputs(leftLight, rightLight);
		m_ctrl_strat->Update();
		m_ctrl_strat->GetOutput(leftForce, rightForce);
		//printf("leftforce: %f, rightforce: %f\n", leftForce, rightForce);
		LeftForce(leftForce);
		RightForce(rightForce);
	}
}

void Vehicle::SetPosition(b2Vec2 pos)
{
	m_position = pos;
	m_body->SetTransform(pos, m_body->GetAngle());
}

void Vehicle::Render(NVGcontext* vg, Renderer & r)
{
	leftSensor.Render(vg, r);
	rightSensor.Render(vg, r);
	r.RenderVehicle(vg, *this);
}

void Vehicle::EnableControl()
{
	m_controllerEnabled = true;
}

void Vehicle::DisableControl()
{
	m_controllerEnabled = false;
}

bool Vehicle::ControlStatus()
{
	return m_controllerEnabled;
}
