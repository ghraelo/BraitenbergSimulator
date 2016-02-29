#include "Raycaster.h"

#include "MathUtils.h"
#include "LightRayCastCallback.h"
#include <cassert>

Raycaster::Raycaster()
	:m_bounds(b2Vec2(1,1),b2Vec2(-1,-1))
{
}

Raycaster::Raycaster(b2World* world, b2Vec2 castPoint, Rectangle bounds)
	:m_castPoint(castPoint), m_bounds(bounds), m_world(world)
{
}
/*
Raycast to all world vertices and bounding box coners to create a smooth visibility polygon
*/
void Raycaster::Cast()
{
	//destroy old raycast polygon
	m_rayCastPolygon.clear();

	std::vector<b2Vec2> vertices = GetVertices();
	//add bounding rectangle corners
	vertices.push_back(m_bounds.m_bottomLeft);
	vertices.push_back(m_bounds.m_topLeft);
	vertices.push_back(m_bounds.m_topRight);
	vertices.push_back(m_bounds.m_bottomRight);

	m_rayCastPolygon = DoRays(vertices);
	
	//sort clockwise by ray angle
	std::sort(m_rayCastPolygon.begin(), m_rayCastPolygon.end(),
		[&](b2Vec2 a, b2Vec2 b) {

		float a_angle = atan2(a.y - m_castPoint.y, a.x - m_castPoint.x);
		float b_angle = atan2(b.y - m_castPoint.y, b.x - m_castPoint.x);
		if (a_angle == b_angle)
			return b2Distance(a, m_castPoint) > b2Distance(b, m_castPoint);
		else
			return a_angle > b_angle;
	}
	);

}

/*
Raycast within the clockwise angle range angle1 to angle2
*/
void Raycaster::Cast(float angle1, float angle2)
{
	//assert(angle1 > angle2);

	m_rayCastPolygon.clear();

	std::vector<b2Vec2> vertices = GetVertices();

	//filter vertices by angle
	std::vector<b2Vec2>::iterator it = std::remove_if(vertices.begin(), vertices.end(),
		[&](b2Vec2 vertex) {
			float angle = atan2(vertex.y - m_castPoint.y, vertex.x - m_castPoint.x);

			return !MathUtils::AngleWithinRange(angle, angle1, angle2);

		}
	);
	vertices.erase(it, vertices.end());

	//add boundary points
	b2Vec2 angle1Dir(cos(angle1), sin(angle1));
	angle1Dir.Normalize();
	b2Vec2 angle2Dir(cos(angle2), sin(angle2));
	angle2Dir.Normalize();

	vertices.push_back(m_castPoint + 100 * angle1Dir);
	vertices.push_back(m_castPoint + 100 * angle2Dir);

	//add bounding rectangle corner if within angle range
	std::vector<b2Vec2> corners = m_bounds.GetPoints();

	for (auto& corner : corners)
	{
		float angle = atan2(corner.y - m_castPoint.y, corner.x - m_castPoint.x);
		if (MathUtils::AngleWithinRange(angle,angle1,angle2))
		{
			vertices.push_back(corner);
		}
	}

	m_rayCastPolygon = DoRays(vertices);

	if(angle1 < 0 && angle2 > 0)
		m_rayCastPolygon.push_back(m_castPoint);

	std::sort(m_rayCastPolygon.begin(), m_rayCastPolygon.end(),
		[&](b2Vec2 a, b2Vec2 b) {
		float a_angle = atan2(a.y - m_castPoint.y, a.x - m_castPoint.x);
		float b_angle = atan2(b.y - m_castPoint.y, b.x - m_castPoint.x);
		if (a_angle == b_angle)
			return b2Distance(a, m_castPoint) > b2Distance(b, m_castPoint);
		else
			return a_angle < b_angle;
	}
	);

	if (!(angle1 < 0 && angle2 > 0))
		m_rayCastPolygon.push_back(m_castPoint);

	//combine array
	//m_rayCastPolygon.insert(m_rayCastPolygon.end(), negativePts.begin(), negativePts.end());
	//m_rayCastPolygon.insert(m_rayCastPolygon.end(), positivePts.begin(), positivePts.end());
	//add centerpoint
}
/*
Get all physics object vertices by iterating through world bodies
*/
std::vector<b2Vec2> Raycaster::GetVertices()
{
	b2Body* bList = m_world->GetBodyList();
	std::vector<b2Vec2> vertices;
	while (bList != NULL)
	{
		if (std::find(m_ignoreBodies.begin(), m_ignoreBodies.end(), bList) == m_ignoreBodies.end())
		{
			//get all fixtures
			b2Fixture* fList = bList->GetFixtureList();

			while (fList != NULL)
			{
				switch (fList->GetType())
				{
				case b2Shape::e_circle:
				{
					b2CircleShape* circle = (b2CircleShape*)fList->GetShape();
					/* Do stuff with a circle shape */
				}
				break;

				case b2Shape::e_polygon:
				{
					b2PolygonShape* poly = (b2PolygonShape*)fList->GetShape();
					/* Do stuff with a polygon shape */
					std::vector<b2Vec2> polyVertices(poly->m_vertices, poly->m_vertices + poly->GetVertexCount());
					for (auto& vert : polyVertices)
					{
						vert = bList->GetWorldPoint(vert);
					}
					vertices.insert(vertices.end(), polyVertices.begin(), polyVertices.end());
				}
				break;
				}
				fList = fList->GetNext();
			}
		}
		bList = bList->GetNext();
	}

	return vertices;
}

std::vector<b2Vec2> Raycaster::DoRays(std::vector<b2Vec2> vertices)
{
	std::vector<b2Vec2> rayCastPoly;
	
	//get furthest vertex
	b2Vec2 max = *std::max_element(vertices.begin(), vertices.end(),
		[&](b2Vec2 a, b2Vec2 b) {
			return b2Distance(a, m_castPoint) < b2Distance(b, m_castPoint);
		}
	);

	for (auto& vec : vertices)
	{
		float rot[3] = { 0, 0.00001f, -0.00001f };
		b2Color col[3] = { b2Color(1, 0, 0) , b2Color(0, 0, 1) , b2Color(0, 0, 1) };

		for (int i = 0; i < 3; i++)
		{
			LightRayCastCallback lrcc;
			//cast ray at vertex
			if(m_ignoreBodies.size() > 0)
				lrcc = LightRayCastCallback(m_ignoreBodies);

			b2Vec2 newVec = MathUtils::RotateAroundPoint(vec, m_castPoint, rot[i]);
			b2Vec2 dirVec = newVec - m_castPoint;
			dirVec.Normalize();

			float dx = 0, dy = 0;

			//get dx,dy to extend ray if it misses (so that it hits the edge of the screen)
			if (dirVec.x > 0 && dirVec.y > 0) //top right
			{
				dx = m_bounds.m_topRight.x - m_castPoint.x;
				dy = m_bounds.m_topRight.y - m_castPoint.y;
			}
			else if (dirVec.x > 0 && dirVec.y < 0) //bottom right
			{
				dx = m_bounds.m_bottomRight.x - m_castPoint.x;
				dy = m_bounds.m_bottomRight.y - m_castPoint.y;
			}
			else if (dirVec.x < 0 && dirVec.y > 0) //top left
			{
				dx = m_bounds.m_topLeft.x - m_castPoint.x;
				dy = m_bounds.m_topLeft.y - m_castPoint.y;
			}
			else if (dirVec.x < 0 && dirVec.y < 0) //bottom left
			{
				dx = m_bounds.m_bottomLeft.x - m_castPoint.x;
				dy = m_bounds.m_bottomLeft.y - m_castPoint.y;
			}

			dx = fabs(dx);
			dy = fabs(dy);

			float edgeDist = b2Vec2(dx, dy).Length();
			float dist = (m_castPoint - max).Length();

			float sf = (edgeDist > dist) ? edgeDist : dist;
			/*
			if (dx < dy)
			{
				sf = dx / dirVec.x;
			}
			else
			{
				sf = dy / dirVec.y;
			}*/

			b2Vec2 npt = m_castPoint + fabs(sf) * dirVec;
			m_world->RayCast(&lrcc, m_castPoint, newVec + 100 * dirVec);

			if (lrcc.m_collided == true)
			{
				rayCastPoly.push_back(lrcc.m_point);
				//g_debugDraw.DrawSegment(point, lrcc.m_point, col[i]);

			}
			else
			{
				rayCastPoly.push_back(npt);
				//g_debugDraw.DrawSegment(point, npt, b2Color(col[i].r*0.5f,col[i].g*0.5f,col[i].g*0.5f,1.0f));
			}
		}
	}

	return rayCastPoly;

}

void Raycaster::SetCastPoint(b2Vec2 castPoint)
{
	m_castPoint = castPoint;
}

void Raycaster::AddIgnoreBody(b2Body * body)
{
	m_ignoreBodies.push_back(body);
}

b2Vec2 Raycaster::GetCastPoint()
{
	return m_castPoint;
}

std::vector<b2Vec2> Raycaster::GetRayCastPoly()
{
	return m_rayCastPolygon;
}

void Raycaster::SetBounds(Rectangle newBounds)
{
	m_bounds = newBounds;
}

Rectangle Raycaster::GetBounds()
{
	return m_bounds;
}