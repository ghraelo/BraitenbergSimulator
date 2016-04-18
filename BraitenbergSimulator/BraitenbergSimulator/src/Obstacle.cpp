#include "Obstacle.h"
#include "Renderer.h"
Obstacle::Obstacle(b2Vec2 position, ObstacleType type, float size, std::string name)
	:SimObject(position, name),m_type(type)

{
	if (type == OT_CIRCLE)
	{
		b2CircleShape circ;
		circ.m_p = b2Vec2_zero;
		circ.m_radius = size;
		m_shape = std::make_unique<b2CircleShape>(circ);
	}
	else if (type == OT_SQUARE)
	{
		b2PolygonShape poly;
		poly.SetAsBox(size / 2, size / 2);
		m_shape = std::make_unique<b2PolygonShape>(poly);
	}
}

void Obstacle::BindPhysics(b2World* world)
{
	theWorld = world;
	b2BodyDef bodyDef;

	bodyDef.position = m_position;
	bodyDef.type = b2_staticBody;
	m_body = (*theWorld).CreateBody(&bodyDef);

	//init fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = m_shape.release();
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	//create fixture
	b2Fixture* theFixture = m_body->CreateFixture(&fixtureDef);

	m_soiPtr = std::make_unique<SimObjectInfo>();
	m_soiPtr->m_obj = this;
	m_soiPtr->m_type = "Obstacle";
	m_body->SetUserData(m_soiPtr.get());

	m_position = m_body->GetPosition();
	int s = 4;
}

void Obstacle::Render(NVGcontext * vg, Renderer & r)
{
	r.RenderObstacle(vg,*this);
}

b2Shape * Obstacle::GetShape()
{
	return m_body->GetFixtureList()->GetShape();
}

ObstacleType Obstacle::GetType()
{
	return m_type;
}
