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

	float rad = m_cam->ConvertWorldToScreen(renderable.GetRadius());

	NVGpaint grad = nvgRadialGradient(vg, pos.x, pos.y, rad/2, rad, nvgRGBA(255, 255, 255, 255), nvgRGBA(0, 0, 0, 0));
	nvgCircle(vg, pos.x, pos.y, rad);

	nvgFillPaint(vg, grad);
	nvgFill(vg);
}

void Renderer::RenderLightSensor(NVGcontext* vg, LightSensor& renderable)
{
	b2Vec2 arc1 = m_cam->ConvertWorldToScreen(renderable.GetArcEnd(10.0f, true));
	b2Vec2 arc2 = m_cam->ConvertWorldToScreen(renderable.GetArcEnd(10.0f, false));
	b2Color white = b2Color(1, 1, 1, 0.5);

	b2Vec2 pos = m_cam->ConvertWorldToScreen(renderable.GetPosition());

	NVGpaint grad1 = nvgLinearGradient(vg, pos.x, pos.y, arc1.x, arc1.y, nvgRGBA(255,255,255,255), nvgRGBA(255, 255, 255, 0));
	NVGpaint grad2 = nvgLinearGradient(vg, pos.x, pos.y, arc2.x, arc2.y, nvgRGBA(255, 255, 255, 255), nvgRGBA(255, 255, 255, 0));

	nvgBeginPath(vg); 
	nvgMoveTo(vg, pos.x,pos.y);
	nvgLineTo(vg, arc1.x,arc1.y);
	nvgStrokePaint(vg, grad1);
	nvgStroke(vg);

	nvgBeginPath(vg);
	nvgMoveTo(vg, pos.x,pos.y);
	nvgLineTo(vg, arc2.x, arc2.y);
	nvgStrokePaint(vg, grad2);
	nvgStroke(vg);

	//draw visibility polygon
}

void Renderer::SetCamera(Camera * cam)
{
	m_cam = cam;
}
