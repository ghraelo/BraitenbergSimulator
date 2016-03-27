#pragma once

#include <Box2D\Box2D.h>
#include <nanovg\nanovg.h>

#include "Rectangle.h"

class Camera
{
public :
	Camera(float screenWidth, float screenHeight);
	b2Vec2 ConvertWorldToScreen(const b2Vec2& worldPt);
	b2Vec2 ConvertScreenToWorld(const b2Vec2& screenPt);
	float ConvertWorldToScreen(float pt);
	Rectangle GetRect();
	void SetOrigin(b2Vec2 newOrigin);
	b2Vec2 GetOrigin();
	void SetZoom(float newZoom);
	float GetZoom();
private:
	float m_screenWidth;
	float m_screenHeight;
	float m_worldScale = 75.0f;
	float m_zoom;
	b2Vec2 m_origin;
};

typedef std::unique_ptr<Camera> CameraPtr;