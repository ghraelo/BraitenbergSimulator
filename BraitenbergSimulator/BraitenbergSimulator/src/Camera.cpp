#include "Camera.h"


Camera::Camera(float screenWidth, float screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

b2Vec2 Camera::ConvertWorldToScreen(const b2Vec2 & worldPt)
{
	float ratio = m_screenHeight / m_screenWidth;

	float x = (worldPt.x) * m_screenWidth / worldScale + m_screenWidth / 2;
	float y = (worldPt.y) /ratio * m_screenHeight / worldScale - m_screenHeight;

	return b2Vec2(x,y);
}

b2Vec2 Camera::ConvertScreenToWorld(const b2Vec2 & screenPt)
{
	float ratio = m_screenHeight / m_screenWidth;

	float x = (screenPt.x - m_screenWidth / 2) * worldScale / m_screenWidth;
	float y = (screenPt.y + m_screenHeight) * ratio * worldScale / m_screenHeight;

	return b2Vec2(x,y);
}

float Camera::ConvertWorldToScreen(float pt)
{
	return pt*m_screenWidth/worldScale;
}

Rectangle Camera::GetRect()
{
	return Rectangle(b2Vec2(m_screenWidth, 0), b2Vec2(0, m_screenHeight));
}