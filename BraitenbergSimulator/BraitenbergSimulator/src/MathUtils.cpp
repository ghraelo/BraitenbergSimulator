#include "MathUtils.h"

float MathUtils::GetAngle(b2Vec2 & a, b2Vec2 & b)
{
	a.Normalize();
	b.Normalize();

	float costheta = b2Dot(a, b) / (a.Length() * b.Length());

	return acosf(costheta);
}
bool MathUtils::IsWithinCircle(b2Vec2 & circleCenter, float circleRadius, b2Vec2 & point)
{
	b2Vec2 test = point - circleCenter;
	if (test.LengthSquared() < circleRadius*circleRadius)
	{
		return true;
	}
	else
		return false;
}

b2Vec2 MathUtils::GetPointProjection(b2Vec2 & lineA, b2Vec2 & lineB, b2Vec2 & point)
{
	//calculate vector eqn of line
	b2Vec2 a = lineA;
	b2Vec2 v = lineB - lineA;
	b2Vec2 w = a - point;
	b2Vec2 projection = (b2Dot(v, w) / b2Dot(v, v)) * v;

	return point + w - projection;
}

void MathUtils::GetIntersections(b2Vec2 & aPos, b2Vec2 & bPos, float lightRadius, b2Vec2 & intersection1, b2Vec2 & intersection2)
{
	b2Vec2 P0 = aPos;
	b2Vec2 P1 = bPos;
	//calculate distance
	float d = b2Distance(P0, P1);
	//r0 = d

	//a = (r0^2 - r1^2 + d^2) / 2d
	float a = (d*d - lightRadius*lightRadius + d*d) / (2 * d);
	//h^2 = ro^2 - a^2
	float h = sqrtf(d*d - a*a);
	b2Vec2 P2 = aPos + (a / d) * (P1 - P0);

	float x_comp = (h / d) * (P1.y - P0.y);
	float y_comp = (h / d) * (P1.x - P0.x);


	intersection1 = b2Vec2(P2.x + x_comp, P2.y - y_comp);
	intersection2 = b2Vec2(P2.x - x_comp, P2.y + y_comp);
}
