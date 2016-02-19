#pragma once

#include "Vehicle.h"
#include "LightSource.h"
#include "BarViewElement.h"

class Renderer
{
public:
	Renderer();
	void RenderVehicle(Vehicle& renderable);
	void RenderLightSource(LightSource& renderable);
	void RenderLightSensor(LightSensor& renderable);
	void Renderer::RenderBarViewElement(BarViewElement& renderable);
};