#pragma once

#include "Vehicle.h"
#include "LightSource.h"
#include "Camera.h"
#include "Boundary.h"

#include <nanovg\nanovg.h>

class Renderer
{
public:
	Renderer();
	void RenderVehicle(NVGcontext* vg, Vehicle& renderable);
	void RenderLightSource(NVGcontext* vg, LightSource& renderable);
	void RenderLightSensor(NVGcontext* vg, LightSensor& renderable);
	void RenderBoundary(NVGcontext *vg, Boundary& renderable);

	void SetCamera(Camera* cam);
private:
	Camera* m_cam;
};