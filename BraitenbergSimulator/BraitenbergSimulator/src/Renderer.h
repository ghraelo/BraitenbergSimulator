#pragma once

#include "Vehicle.h"
#include "LightSource.h"
#include "Camera.h"
#include "Boundary.h"
#include "SimManager.h"
#include "Obstacle.h"

#include <nanovg\nanovg.h>

class Renderer
{
public:
	Renderer();
	void RenderVehicle(NVGcontext* vg, Vehicle& renderable);
	void RenderLightSource(NVGcontext* vg, LightSource& renderable);
	void RenderLightSensor(NVGcontext* vg, LightSensor& renderable);
	void RenderBoundary(NVGcontext *vg, Boundary& renderable);
	void RenderObstacle(NVGcontext *vg, Obstacle& renderable);
	void SetCamera(Camera* cam);
private:
	Camera* m_cam;
	void DrawPolygon(NVGcontext * vg, const std::vector<b2Vec2>& poly, const NVGcolor& color);
	void DrawFilledPolygon(NVGcontext * vg, const std::vector<b2Vec2>& poly, const NVGcolor& color);
	float flashTimer = 60.0f;
	float flashTimerMax = 60.0f;
	bool flashing = false;
};