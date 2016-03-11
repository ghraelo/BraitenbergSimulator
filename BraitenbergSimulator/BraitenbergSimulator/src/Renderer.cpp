#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::RenderVehicle(Vehicle & renderable)
{

}

void Renderer::RenderLightSource(LightSource & renderable)
{
}

void Renderer::RenderLightSensor(LightSensor& renderable)
{
	b2Vec2 arc1 = renderable.GetArcEnd(10.0f, true);
	b2Vec2 arc2 = renderable.GetArcEnd(10.0f, false);
	b2Color white = b2Color(1, 1, 1, 0.5);
}