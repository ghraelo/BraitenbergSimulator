#include "Camera.h"


Camera::Camera(float screenWidth, float screenHeight)
	:m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_origin(b2Vec2(screenWidth/2,screenHeight)), m_zoom(1.0f)

{
}

b2Vec2 Camera::ConvertWorldToScreen(const b2Vec2 & worldPt)
{
	float ratio = m_screenHeight / m_screenWidth;

	float x = (worldPt.x) * m_screenWidth / (m_worldScale * m_zoom) + m_origin.x;
	float y = (worldPt.y) /ratio * m_screenHeight / (m_worldScale * m_zoom) - m_origin.y;

	return b2Vec2(x,y);
}

b2Vec2 Camera::ConvertScreenToWorld(const b2Vec2 & screenPt)
{
	float ratio = m_screenHeight / m_screenWidth;

	float x = (screenPt.x - m_origin.x / 2) * (m_worldScale * m_zoom) / m_screenWidth;
	float y = (screenPt.y + m_origin.y) * ratio * (m_worldScale * m_zoom) / m_screenHeight;

	return b2Vec2(x,y);
}

float Camera::ConvertWorldToScreen(float pt)
{
	return pt*m_screenWidth/ (m_worldScale * m_zoom);
}

Rectangle Camera::GetRect()
{
	return Rectangle(b2Vec2(m_screenWidth, 0), b2Vec2(0, m_screenHeight));
}

void Camera::SetOrigin(b2Vec2 newOrigin)
{
	m_origin = newOrigin;
}

b2Vec2 Camera::GetOrigin()
{
	return m_origin;
}

void Camera::SetZoom(float newZoom)
{
	m_zoom = newZoom;
}

float Camera::GetZoom()
{
	return m_zoom;
}
