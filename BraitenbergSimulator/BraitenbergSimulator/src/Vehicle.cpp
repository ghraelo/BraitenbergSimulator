#include "Vehicle.h"
#include "Renderer.h"
#include "SimObjectInfo.h"

Vehicle::Vehicle()
	:leftSensor(this), rightSensor(this)
{
}

Vehicle::Vehicle(sensorInfo leftInfo, sensorInfo rightInfo)
	:leftSensor(this, leftInfo), rightSensor(this, rightInfo)
{
}

Vehicle::Vehicle(Vehicle const & other)
	:m_body(other.m_body), bodyDef(other.bodyDef),vehicleShape(other.vehicleShape),fixtureDef(other.fixtureDef),theWorld(other.theWorld), m_physicsBound(other.m_physicsBound),leftSensor(this,leftSensor.GetSensorInfo()), rightSensor(this, rightSensor.GetSensorInfo())
{
	leftSensor = LightSensor(this,other.leftSensor.GetSensorInfo());
	rightSensor = LightSensor(this, other.rightSensor.GetSensorInfo());
	
	//reset body data to point to correct object
	if(m_body != nullptr)
		SetUserData();
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

void Vehicle::Update(std::vector<LightSource> ls)
{
	leftSensor.GetLight(ls);
	rightSensor.GetLight(ls);
}

void Vehicle::Render(Renderer & r)
{
	leftSensor.Render(r);
	rightSensor.Render(r);
	r.RenderVehicle(*this);
}
