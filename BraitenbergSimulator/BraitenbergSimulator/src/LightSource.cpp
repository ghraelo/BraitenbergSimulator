#include "LightSource.h"
#include "MyRayCastCallback.h"


LightSource::LightSource(b2World* world, b2Vec2 position)
	: m_world(world), light_position(position), light_intensity(10000)
{

	//init body
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position = light_position;

	m_body = (*m_world).CreateBody(&bodyDef);

	float maxRadius = sqrt(light_intensity / 255);
	light_boundary.m_p = m_body->GetPosition();
	light_boundary.m_radius = maxRadius;

	//init fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &light_boundary;
	fixtureDef.isSensor = true;
	//create fixture
	b2Fixture* theFixture = m_body->CreateFixture(&fixtureDef);
	FixtureData* fd = new FixtureData;
	fd->m_type = 0;
	fd->m_ptr = this;
	theFixture->SetUserData(fd);
}

LightSource::~LightSource()
{

}

b2Vec2 LightSource::get_position()
{
	return m_body->GetWorldPoint(m_body->GetPosition());
}

void LightSource::set_position(b2Vec2 position)
{
	light_position = position;
	light_boundary.m_p.Set(light_position.x, light_position.y);
}

float LightSource::getIntensity()
{
	return light_intensity;
}

