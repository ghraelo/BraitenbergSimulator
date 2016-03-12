#include "SimObject.h"

#include "Renderer.h"

SimObject::SimObject()
	:m_position(b2Vec2_zero)
{

}

SimObject::SimObject(b2Vec2 position)
	:m_position(position)
{

}


b2Vec2 SimObject::GetPosition()
{
	return m_position;
}

void SimObject::Update()
{
}

void SimObject::Render(NVGcontext* vg, Renderer & r)
{
}
