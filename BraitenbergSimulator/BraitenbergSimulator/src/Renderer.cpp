#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::RenderVehicle(NVGcontext* vg, Vehicle & renderable)
{
	b2Fixture* f_list = renderable.m_body->GetFixtureList();
	b2PolygonShape* shape = (b2PolygonShape*)f_list->GetShape();

	nvgBeginPath(vg);
	b2Vec2 start = m_cam->ConvertWorldToScreen(renderable.m_body->GetWorldPoint(shape->GetVertex(0)));
	nvgMoveTo(vg, start.x,start.y);

	for (int i = 1; i < shape->GetVertexCount();i++)
	{
		b2Vec2 pt = m_cam->ConvertWorldToScreen(renderable.m_body->GetWorldPoint(shape->GetVertex(i)));
		nvgLineTo(vg,pt.x,pt.y);

	}
	nvgStrokeWidth(vg, 1);
	nvgStrokeColor(vg, nvgRGBA(255,0,0,255));
	nvgFillColor(vg, nvgRGBA(180, 0, 0, 255));
	nvgFill(vg);
	nvgStroke(vg);
}

void Renderer::RenderLightSource(NVGcontext* vg, LightSource & renderable)
{
	nvgBeginPath(vg);
	b2Vec2 pos = m_cam->ConvertWorldToScreen(renderable.GetPosition());
	printf("pos : %f,%f\n", pos.x, pos.y);
	nvgCircle(vg, pos.x, pos.y, m_cam->ConvertWorldToScreen(renderable.GetRadius()));
	nvgStrokeWidth(vg, 1);
	nvgStrokeColor(vg, nvgRGBA(0, 255, 0, 255));
	nvgFillColor(vg, nvgRGBA(0, 180, 0, 255));
	nvgFill(vg);
	nvgStroke(vg);
}

void Renderer::RenderLightSensor(NVGcontext* vg, LightSensor& renderable)
{
	b2Vec2 arc1 = renderable.GetArcEnd(10.0f, true);
	b2Vec2 arc2 = renderable.GetArcEnd(10.0f, false);
	b2Color white = b2Color(1, 1, 1, 0.5);
}

void Renderer::SetCamera(Camera * cam)
{
	m_cam = cam;
}
