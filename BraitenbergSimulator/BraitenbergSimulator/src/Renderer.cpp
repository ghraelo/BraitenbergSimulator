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

	NVGcolor grn = nvgRGBA(0, 255, 0, 255);
	nvgCircle(vg, pos.x, pos.y, rad);
	nvgFillColor(vg, grn);
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
	DrawFilledPolygon(vg,renderable.GetRayCastPolygon(), nvgRGBA(255, 255, 255, 128));

}

void Renderer::RenderBoundary(NVGcontext * vg, Boundary & renderable)
{
	const float dashPercent = 0.05f;

	NVGcolor theColor = nvgLerpRGBA(nvgRGBA(255, 255, 255, 255), nvgRGBA(255, 182, 193, 255), (flashTimer / flashTimerMax));
	std::vector<b2Vec2> rectPts = renderable.GetRect().GetPoints();
	rectPts.push_back(rectPts[0]);

	b2Vec2 startPt = m_cam->ConvertWorldToScreen(rectPts[0]);

	for (int i = 1; i < rectPts.size(); i++)
	{
		b2Vec2 endPt = m_cam->ConvertWorldToScreen(rectPts[i]);

		float lineLength = b2Distance(startPt, endPt);
		float dashLength = dashPercent * lineLength;
		b2Vec2 lineDir = endPt - startPt;
		lineDir.Normalize();
		int dashNum = ceil(lineLength / (dashLength) + 1);

		nvgBeginPath(vg);
		nvgMoveTo(vg, startPt.x, startPt.y);

		for (int i = 0; i < dashNum; i++)
		{
			b2Vec2 pos = startPt + i * dashLength * lineDir;
			
			if (i == 0 || i % 2 == 0)
			{
				nvgLineTo(vg, pos.x, pos.y);
			}
			else
			{
				nvgMoveTo(vg, pos.x, pos.y);
			}
		}

		nvgStrokeWidth(vg, 1);
		nvgStrokeColor(vg, theColor);
		nvgStroke(vg);

		startPt = endPt;
	}

	if (flashTimer >= flashTimerMax)
	{
		flashing = false;
		flashTimer = flashTimerMax;
	}

	if (renderable.IsColliding())
	{
		flashing = true;
		flashTimer = 0.0f;
	}

	if (flashing)
	{
		flashTimer = flashTimer + 1.0;
	}

}

void Renderer::RenderObstacle(NVGcontext * vg, Obstacle & renderable)
{
	ObstacleType type = renderable.GetType();
	if (type == OT_CIRCLE)
	{
		b2CircleShape* circ = (b2CircleShape*)renderable.GetShape();

		nvgBeginPath(vg);
		b2Vec2 pos = m_cam->ConvertWorldToScreen(renderable.m_body->GetWorldPoint(circ->m_p));

		b2Vec2 pos2 = m_cam->ConvertWorldToScreen(renderable.GetPosition() + b2Vec2(0, circ->m_radius));

		float rad = b2Distance(pos, pos2);

		NVGpaint grad = nvgRadialGradient(vg, pos.x, pos.y, rad / 2, rad, nvgRGBA(255, 255, 255, 255), nvgRGBA(0, 0, 0, 0));
		
		nvgCircle(vg, pos.x, pos.y, rad);
		nvgFillColor(vg, nvgRGBA(0, 0, 255, 255));
		nvgFill(vg);

	}
	else
	{
		b2PolygonShape* shape = (b2PolygonShape*)renderable.GetShape();
		std::vector<b2Vec2> vertices;
		vertices.assign(shape->m_vertices, shape->m_vertices + shape->GetVertexCount());
		std::transform(vertices.begin(), vertices.end(), vertices.begin(),
			[&](b2Vec2 a) {
			return renderable.m_body->GetWorldPoint(a);
		}
		);

		DrawPolygon(vg, vertices, nvgRGBA(0, 0, 255, 255));
		DrawFilledPolygon(vg, vertices, nvgRGBA(0, 0, (unsigned char)(255 * 0.75f), 255));
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
