#pragma once

#include <Box2D\Box2D.h>
#include <vector>

#define M_PI 3.141592654
class MathUtils
{
public:
	static float GetAngle(b2Vec2& a, b2Vec2& b);
	static bool  IsWithinCircle(b2Vec2& circleCenter, float circleRadius, b2Vec2& point);
	static b2Vec2 GetPointProjection(b2Vec2& lineA, b2Vec2& lineB, b2Vec2& point);
	static void GetIntersections(b2Vec2& aPos, b2Vec2& bPos, float lightRadius, b2Vec2& intersection1, b2Vec2& intersection2);
	static b2Vec2 GetCentroid(std::vector<b2Vec2>& points);
	static b2Vec2 RotateAroundPoint(b2Vec2 rotated, b2Vec2 centre, float angle);
	static int GetIntersectionLines(const b2Vec2& a1, const b2Vec2& a2, const b2Vec2& b1, const b2Vec2& b2, b2Vec2& result);
	static bool SameSide(const b2Vec2& p1, const b2Vec2& p2, const b2Vec2& l1, const b2Vec2& l2);
	static bool InsideTriangle(const b2Vec2& p, const b2Vec2& t1, const b2Vec2& t2, const b2Vec2& t3);
	static bool PointInPoly(const std::vector<b2Vec2>& polygon, const b2Vec2& point);
	static bool AngleWithinRange(float angle, float limit1, float limit2);
	static float AngleToFraction(float min, float angle, float max);
private:
	MathUtils();
};