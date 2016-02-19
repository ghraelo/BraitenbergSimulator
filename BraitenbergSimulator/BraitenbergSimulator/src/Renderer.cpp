#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::RenderVehicle(Vehicle & renderable)
{

}

void Renderer::RenderLightSource(LightSource & renderable)
{
	g_debugDraw.DrawCircle(renderable.GetPosition(), renderable.GetRadius(), b2Color(0, 1, 0));
}

void Renderer::RenderLightSensor(LightSensor& renderable)
{
	b2Vec2 arc1 = renderable.GetArcEnd(10.0f, true);
	b2Vec2 arc2 = renderable.GetArcEnd(10.0f, false);
	b2Color white = b2Color(1, 1, 1, 0.5);

	g_debugDraw.DrawSegmentFade(renderable.GetPosition(), arc1, white);
	g_debugDraw.DrawSegmentFade(renderable.GetPosition(), arc2, white);
}

void Renderer::RenderBarViewElement(BarViewElement& renderable)
{

}