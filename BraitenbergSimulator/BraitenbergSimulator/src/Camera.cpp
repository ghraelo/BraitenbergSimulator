#include "Camera.h"

Camera::Camera(float screenWidth, float screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

b2Vec2 Camera::ConvertWorldToScreen(const b2Vec2 & worldPt)
{
	float ratio = m_screenHeight / m_screenWidth;

	float x = worldPt.x * m_screenWidth / worldScale;
	float y = worldPt.y /ratio * m_screenHeight / worldScale;

	return b2Vec2(x,y);
}

float Camera::ConvertWorldToScreen(float pt)
{
	return pt*m_screenWidth/worldScale;
}

