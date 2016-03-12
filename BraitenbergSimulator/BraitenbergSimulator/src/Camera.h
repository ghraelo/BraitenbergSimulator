#pragma once

#include <Box2D\Box2D.h>
#include <nanovg\nanovg.h>

class Camera
{
public :
	Camera(float screenWidth, float screenHeight);
	b2Vec2 ConvertWorldToScreen(const b2Vec2& worldPt);
	float ConvertWorldToScreen(float pt);
private:
	float m_screenWidth;
	float m_screenHeight;
	float worldScale = 75.0f;
};

typedef std::unique_ptr<Camera> CameraPtr;