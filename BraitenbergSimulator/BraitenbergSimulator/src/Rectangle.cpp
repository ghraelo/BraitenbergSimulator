#include "Rectangle.h"

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(b2Vec2 topRight, b2Vec2 bottomLeft)
	:m_topLeft(b2Vec2(bottomLeft.x, topRight.y)), m_topRight(topRight), m_bottomLeft(bottomLeft), m_bottomRight(b2Vec2(topRight.x, bottomLeft.y))
{

}

std::vector<b2Vec2> Rectangle::GetPoints()
{
	std::vector<b2Vec2> temp;
	temp.push_back(m_bottomLeft);
	temp.push_back(m_topLeft);
	temp.push_back(m_topRight);
	temp.push_back(m_bottomRight);
	return temp;
}

bool Rectangle::Inside(const b2Vec2 & point)
{
	return (point.x > m_bottomLeft.x && point.x < m_topRight.x && point.y < m_topRight.y && point.y > m_bottomLeft.y);
}
