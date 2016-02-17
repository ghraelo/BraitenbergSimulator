#pragma once

#include "Box2D\Box2D.h"
#include "LightSource.h"

struct FixtureData
{
	int m_type;

	void* m_ptr;
};

// This class captures the closest hit shape.

class MyRayCastCallback : public b2RayCastCallback

{

public:

	MyRayCastCallback()

	{
		m_occured = false;
		m_fixture = NULL;

	}



	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_source = static_cast<LightSource*>(fixture->GetUserData());

		m_fixture = fixture;
		FixtureData* fd = static_cast<FixtureData*>(fixture->GetUserData());

		m_type = fd->m_type;
		if (fd->m_type == 0)
		{
			m_source = static_cast<LightSource*>(fd->m_ptr);
			m_fraction = fraction;
		}
		else
		{

			m_fraction = -1;
		}

		m_point = point;

		m_normal = normal;
		m_occured = true;
		return m_fraction;

	}
	int m_type;
	
	bool m_occured;

	LightSource* m_source;

	b2Fixture* m_fixture;

	b2Vec2 m_point;

	b2Vec2 m_normal;

	float32 m_fraction;

};

