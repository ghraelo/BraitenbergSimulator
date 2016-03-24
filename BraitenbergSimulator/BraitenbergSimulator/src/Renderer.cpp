#include "Renderer.h"

Renderer::Renderer()
{
}

void Renderer::RenderVehicle(NVGcontext* vg, Vehicle & renderable)
{
	b2Fixture* f_list = renderable.m_body->GetFixtureList();
	b2PolygonShape* shape = (b2PolygonShape*)f_list->GetShape();

	/*nvgBeginPath(vg);
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
	nvgStroke(vg);*/
	std::vector<b2Vec2> vertices;
	vertices.assign(shape->m_vertices, shape->m_vertices + shape->GetVertexCount());
	std::transform(vertices.begin(), vertices.end(), vertices.begin(),
		[&](b2Vec2 a) {
			return renderable.m_body->GetWorldPoint(a);
		}
	);

	DrawPolygon(vg, vertices, nvgRGBA(255, 0, 0, 255));
	DrawFilledPolygon(vg, vertices, nvgRGBA((unsigned char)(255*0.75f), 0, 0, 255));

}

void Renderer::RenderLightSource(NVGcontext* vg, LightSource & renderable)
{
	nvgBeginPath(vg);
	b2Vec2 pos = m_cam->ConvertWorldToScreen(renderable.GetPosition());

	b2Vec2 pos2 = m_cam->ConvertWorldToScreen(renderable.GetPosition() + b2Vec2(0, renderable.GetRadius()));

	float rad = b2Distance(pos, pos2);

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
	//DrawFilledPolygon(vg,renderable.GetRayCastPolygon(), nvgRGBA(255, 255, 255, 128));

}

void Renderer::RenderBoundary(NVGcontext * vg, Boundary & renderable)
{
	int i = 0;
	NVGcolor col[4];
	col[0] = nvgRGBA(0, 0, 169, 255);
	col[1] = nvgRGBA(255, 0, 169, 255);
	col[2] = nvgRGBA(0, 255, 169, 255);
	col[3] = nvgRGBA(255, 0, 0, 255);

	for (auto& rect : renderable.aabbs)
	{
		b2Vec2 tl = m_cam->ConvertWorldToScreen(rect.m_topLeft);
		b2Vec2 br = m_cam->ConvertWorldToScreen(rect.m_bottomRight);

		b2Vec2 midL(tl.x, tl.y + (tl.y - br.y) / 2);
		b2Vec2 midR(br.x, tl.y + (tl.y - br.y) / 2);

		b2Vec2 midT(tl.x + (tl.x - br.x) / 2, tl.y);
		b2Vec2 midB(tl.x + (tl.x - br.x) / 2, br.y);

		NVGpaint thePaint;

		switch (i)
		{
			case 0:		
				thePaint = nvgLinearGradient(vg, midT.x, midT.y, midB.x, midB.y, nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 255, 255, 180));
				break;
			case 1:
				thePaint = nvgLinearGradient(vg, midB.x, midB.y, midT.x, midT.y, nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 255, 255, 180));
				break;
			case 2:	
				thePaint = nvgLinearGradient(vg, midL.x, midL.y, midR.x, midR.y, nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 255, 255, 180));
				break;
			case 3:
				thePaint = nvgLinearGradient(vg, midR.x, midR.y, midL.x, midL.y, nvgRGBA(0, 0, 0, 0), nvgRGBA(0, 255, 255, 180));
				break;
		}
		nvgBeginPath(vg);

		nvgRect(vg, tl.x,tl.y,br.x - tl.x, br.y - tl.y);
		nvgFillPaint(vg, thePaint);
		nvgFill(vg);
		i++;
	}

}

void Renderer::SetCamera(Camera * cam)
{
	m_cam = cam;
}

void Renderer::DrawPolygon(NVGcontext * vg, const std::vector<b2Vec2>& poly, const NVGcolor& color)
{
	nvgBeginPath(vg);
	b2Vec2 start = m_cam->ConvertWorldToScreen(poly[0]);
	nvgMoveTo(vg, start.x, start.y);

	for (unsigned int i = 1; i < poly.size(); i++)
	{
		b2Vec2 pt = m_cam->ConvertWorldToScreen(poly[i]);
		nvgLineTo(vg, pt.x, pt.y);

	}
	nvgStrokeWidth(vg, 1);
	nvgStrokeColor(vg, color);
	nvgStroke(vg);
}

void Renderer::DrawFilledPolygon(NVGcontext * vg, const std::vector<b2Vec2>& poly, const NVGcolor& color)
{
	nvgBeginPath(vg);
	b2Vec2 start = m_cam->ConvertWorldToScreen(poly[0]);
	nvgMoveTo(vg, start.x, start.y);

	for (unsigned int i = 1; i < poly.size(); i++)
	{
		b2Vec2 pt = m_cam->ConvertWorldToScreen(poly[i]);
		nvgLineTo(vg, pt.x, pt.y);

	}
	nvgStrokeWidth(vg, 1);
	//nvgStrokeColor(vg, color);
	nvgFillColor(vg, color);
	nvgFill(vg);
	//nvgStroke(vg);
}
