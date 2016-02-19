#include "Vehicle.h"
#include "Renderer.h"

Vehicle::Vehicle(b2World* world)
	: theWorld(world)
{
	Initialise();
}

Vehicle::Vehicle(b2World * world, sensorInfo leftInfo, sensorInfo rightInfo)
	:theWorld(world)
{
	Initialise();
	leftSensor = LightSensor(m_body, leftInfo);
	rightSensor = LightSensor(m_body, rightInfo);
}

Vehicle::~Vehicle()
{
	theWorld->DestroyBody(m_body);
}

void Vehicle::Initialise()
{
	//init body
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	bodyDef.gravityScale = 0.0f;
	bodyDef.angularDamping = 0.5f;
	bodyDef.linearDamping = 0.5f;
	m_body = (*theWorld).CreateBody(&bodyDef);
	
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

void Vehicle::Render(Renderer & r)
{
	leftSensor.Render(r);
	rightSensor.Render(r);
	r.RenderVehicle(*this);
}
