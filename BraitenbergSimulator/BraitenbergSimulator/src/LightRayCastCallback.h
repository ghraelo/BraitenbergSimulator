#pragma once

#include <Box2D\Box2D.h>
#include <vector>
class LightRayCastCallback : public b2RayCastCallback
{
public:
	LightRayCastCallback()
		:b2RayCastCallback()
	{

	}

	LightRayCastCallback(std::vector<b2Body*> ignoreList)
		:b2RayCastCallback(), m_ignoreList(ignoreList)
	{

	}
	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction)
	{
		m_fixture = fixture;
		m_point = point;
		m_normal = normal;
		m_fraction = fraction;
		if (m_ignoreList.size() > 0)
		{
			if (std::find(m_ignoreList.begin(), m_ignoreList.end(), m_fixture->GetBody()) != m_ignoreList.end())
			{
				return -1;
			}
			else
			{
				m_collided = true;
				return fraction;
			}
		}
		else
		{
			m_collided = true;
			return fraction;
		}
	}

	b2Fixture* m_fixture;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	float32 m_fraction;
	bool m_collided = false;
private:
	std::vector<b2Body*> m_ignoreList;
};