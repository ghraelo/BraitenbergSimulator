#include "LightSource.h"
#include "Renderer.h"

LightSource::LightSource()
	:SimObject(), m_radius(1.0f)
{

}

LightSource::LightSource(b2Vec2 position, float radius)
	:SimObject(position), m_radius(radius)
{
}

float LightSource::GetRadius()
{
	return m_radius;
}

void LightSource::Render(Renderer& r)
{
	r.RenderLightSource(*this);
}

void LightSource::Update()
{

}

