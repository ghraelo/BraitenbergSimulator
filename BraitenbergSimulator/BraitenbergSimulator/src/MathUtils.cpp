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

b2Vec2 MathUtils::GetCentroid(std::vector<b2Vec2>& points)
{
	//xn, yn = x0, y0
	points.push_back(*points.begin());

	float xPt = 0;
	float yPt = 0;
	float area = 0;

	for (int i = 0; i < (points.size() - 1);++i)
	{
		float temp = points[i].x * points[i + 1].y - points[i + 1].x * points[i].y;
		xPt += (points[i].x + points[i + 1].x)*temp;
		yPt += (points[i].y + points[i + 1].y)*temp;
		area += temp;
	}

	area = fabs(area / 2);
	xPt = xPt / (6 * area);
	yPt = yPt / (6 * area);

	return b2Vec2(xPt, yPt);
}

b2Vec2 MathUtils::RotateAroundPoint(b2Vec2 rotated, b2Vec2 centre, float angle)
{
	b2Vec2 ret;
	ret = rotated;
	// translate point back to origin:
	ret.x -= centre.x;
	ret.y -= centre.y;

	// rotate point
	float xnew = ret.x * cos(angle) - ret.y * sin(angle);
	float ynew = ret.x * sin(angle) + ret.y * cos(angle);

	// translate point back:
	ret.x = xnew + centre.x;
	ret.y = ynew + centre.y;
	return ret;
}

int MathUtils::GetIntersectionLines(const b2Vec2& a1, const b2Vec2& a2, const b2Vec2& b1, const b2Vec2& b2, b2Vec2& result)
{
	b2Vec2 r = a2 - a1;
	b2Vec2 s = b2 - b1;

	float rxs = b2Cross(r, s);
	float qpxr = b2Cross(b1 - a1, r);

	float t = b2Cross(b1 - a1, s) / rxs;
	float u = qpxr / rxs;

	if (rxs == 0)
	{
		if (qpxr == 0)
		{
			//collinear
			return -1;
		}
		else
		{
			//parallel
			return 1;
		}
	}
	else
	{
		if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
		{
			//intersection
			result = a1 + t*r;
			return 0;
		}
		else
		{
			//no intersection
			return 2;
		}
	}
}

bool MathUtils::SameSide(const b2Vec2 & p1, const b2Vec2 & p2, const b2Vec2 & l1, const b2Vec2 & l2)
{
	bool ss = ((p1.x - l1.x) * (l2.y - l1.y) - (l2.x - l1.x) * (p1.y - l1.y))
		* ((p2.x - l1.x) * (l2.y - l1.y) - (l2.x - l1.x) * (p2.y - l1.y)) > 0;

	return ss;
}

bool MathUtils::InsideTriangle(const b2Vec2& p, const b2Vec2& t1, const b2Vec2& t2, const b2Vec2& t3)
{
	bool ss1 = SameSide(p, t1, t2, t3);
	bool ss2 = SameSide(p, t2, t1, t3);
	bool ss3 = SameSide(p, t3, t1, t2);

	return ss1 && ss2 && ss3;
}
/*
From https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
*/
bool MathUtils::PointInPoly(const std::vector<b2Vec2>& polygon, const b2Vec2 & point)
{
	int i, j, c = 0;
	for (i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++) {
		if (((polygon[i].y>point.y) != (polygon[j].y>point.y)) &&
			(point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
			c = !c;
	}
	return c;
}

float constrainAngle(float x)
{
	x = fmod(x + 180, 360);
	if (x < 0)
		x += 360;
	return x - 180;
}

bool MathUtils::AngleWithinRange(float angle, float limit1, float limit2)
{
	if (limit1 < 0 && limit2 > 0)
	{
		if (angle < 0)
		{
			return angle < limit1;
		}
		else
		{
			return angle > limit2;
		}
	}
	else
	{
		return (angle < limit1) && (angle > limit2);
	}
}
