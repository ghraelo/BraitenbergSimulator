#include "RepulsionBoundary.h"

#include "Renderer.h"
#include "SimObjectInfo.h"
#include "MathUtils.h"

RepulsionBoundary::RepulsionBoundary(b2World * world, b2Vec2 topLeft, float width, float height, float influence)
	:Boundary(world, topLeft, width, height, influence)
{
	b2AABB theAABB;
	BoundaryQueryCallback theQuery;
	BoundaryData bd;

	//top
	b2Vec2 a = m_topLeft;
	b2Vec2 b = m_topLeft + b2Vec2(m_width, -m_influence);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	b2Vec2 force(0, theAABB.upperBound.y - theAABB.lowerBound.y);
	force.Normalize();


	bd.m_aabb = theAABB;
	bd.m_force = 0.95 * -m_height * force;
	bd.m_type = BT_TOP;
	bd.m_midPt = m_topLeft + b2Vec2(m_width / 2, 0);
	edges.push_back(bd);

	//bottom
	a = m_topLeft + b2Vec2(0, -m_height);
	b = m_topLeft + b2Vec2(m_width, -m_height + m_influence);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));


	force = b2Vec2(0, theAABB.upperBound.y - theAABB.lowerBound.y);
	force.Normalize();

	bd.m_aabb = theAABB;
	bd.m_force = 0.95 * m_height * force;
	bd.m_type = BT_BOTTOM;
	bd.m_midPt = m_topLeft + b2Vec2(m_width / 2, -m_height);
	edges.push_back(bd);



	//left
	a = m_topLeft;
	b = m_topLeft + b2Vec2(m_influence, -m_height);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	force = b2Vec2(theAABB.upperBound.x - theAABB.lowerBound.x, 0);
	force.Normalize();

	bd.m_aabb = theAABB;
	bd.m_force = 0.95 * m_width * force;
	bd.m_type = BT_LEFT;
	bd.m_midPt = m_topLeft + b2Vec2(0, -m_height / 2);
	edges.push_back(bd);


	//right
	a = m_topLeft + b2Vec2(m_width, 0);
	b = m_topLeft + b2Vec2(m_width - m_influence, -m_height);

	theAABB.lowerBound = b2Vec2(fmin(a.x, b.x), fmin(a.y, b.y));
	theAABB.upperBound = b2Vec2(fmax(a.x, b.x), fmax(a.y, b.y));

	force = b2Vec2(theAABB.lowerBound.x - theAABB.upperBound.x, 0);
	force.Normalize();

	bd.m_aabb = theAABB;
	bd.m_force = 0.95 * m_width * force;
	bd.m_type = BT_RIGHT;
	bd.m_midPt = m_topLeft + b2Vec2(m_width, -m_height / 2);
	edges.push_back(bd);
}

void RepulsionBoundary::Update(double time)
{
	m_isColliding = false;
	for (auto& edge : edges)
	{
		BoundaryQueryCallback theQuery;
		m_world->QueryAABB(&theQuery, edge.m_aabb);

		if (!theQuery.m_foundBodies.empty())
		{
			m_isColliding = true;
			for (auto& body : theQuery.m_foundBodies)
			{
				void* info = body->GetUserData();
				SimObjectInfo* s = (SimObjectInfo*)info;

				if (s->m_type == "Vehicle")
				{
					Vehicle* v = (Vehicle*)s->m_obj;

					float val = 0.0f;
					b2Vec2 dir = v->m_body->GetWorldVector(b2Vec2(0, 1));
					float veh_angle = MathUtils::WrapAngle(v->m_body->GetAngle());
					//printf("vehicle angle: %f\n", veh_angle * 180 / M_PI);
					switch (edge.m_type)
					{
						case BT_BOTTOM:
							val = abs(veh_angle) - M_PI / 2;
							v->SetAngle(veh_angle > 0 ? M_PI / 2 - val : -(M_PI / 2 - val));
							v->SetPosition(v->GetPosition() + b2Vec2(0, 0.1));
							break;
						case BT_TOP:
							val = M_PI/2 - abs(veh_angle);
							v->SetAngle(veh_angle > 0 ? M_PI / 2 + val : -(M_PI / 2 + val));
							v->SetPosition(v->GetPosition() + b2Vec2(0, -0.1));
							break;
						case BT_LEFT:
							v->SetPosition(v->GetPosition() + b2Vec2(0.1, 0));
							v->SetAngle(-veh_angle);
							break;
						case BT_RIGHT:
							v->SetPosition(v->GetPosition() + b2Vec2(-0.1, 0));
							v->SetAngle(-veh_angle);
							break;
					}
					veh_angle = MathUtils::WrapAngle(v->m_body->GetAngle());
					//printf("new vehicle angle: %f\n", veh_angle * 180 / M_PI);
					//printf("dist: %f\n", dist);
					//f = (1/dist) * f;
					//printf("force: (%f,%f)\n", f.x,f.y);

					//printf("current position: (%f,%f)\nafp.second: (%f,%f)\n", v->GetPosition().x, v->GetPosition().y, afp.second.x,afp.second.y);
					OnCollision(edge.m_type, v->GetPosition(), time, v);
					


				}
				//repel body
				//b2Vec2 point = body->GetWorldPoint(b2Vec2(0, 2.0f));
				//body->ApplyForce(afp.second, point, true);
			}

			theQuery.m_foundBodies.clear();
		}
	}
}

void RepulsionBoundary::Render(NVGcontext * vg, Renderer & r)
{
	r.RenderBoundary(vg, *this);
}
