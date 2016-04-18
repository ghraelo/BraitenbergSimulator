#include "LightSource.h"
#include "Renderer.h"

LightSource::LightSource()
	:SimObject(), m_radius(1.0f)
{

}

LightSource::LightSource(b2Vec2 position, float radius, std::string name)
	:SimObject(position, name), m_radius(radius)
{

}

float LightSource::GetRadius()
{
	return m_radius;
}

void LightSource::Render(NVGcontext* vg, Renderer& r)
{
	r.RenderLightSource(vg, *this);
}

void LightSource::Update()
{

}

