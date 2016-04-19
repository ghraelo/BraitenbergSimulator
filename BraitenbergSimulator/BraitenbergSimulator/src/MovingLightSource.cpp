#include "MovingLightSource.h"

MovingLightSource::MovingLightSource(float radius, std::string name, std::vector<b2Vec2> path, float speed)
	:LightSource(path[0],radius,name), m_path(path), m_speed(speed)
{
}

void MovingLightSource::Update()
{
	//if arrived at point, increment target index
	if (WithinRange(m_position,m_path[targetIndex]))
	{
		//increment index
		if (targetIndex < m_path.size() - 1)
			targetIndex++;
		else
		{
			targetIndex = 0;
		}
	}

	//calculate direction to move
	b2Vec2 dir = m_path[targetIndex] - m_position;
	dir.Normalize();
	
	//move
	m_position += m_speed * dir;	
}

bool MovingLightSource::WithinRange(b2Vec2 src, b2Vec2 target)
{
	if (src.x < target.x - m_speed || src.x > target.x + m_speed)
		return false;

	if (src.y < target.y - m_speed || src.y > target.y + m_speed)
		return false;
	
	return true;
}
