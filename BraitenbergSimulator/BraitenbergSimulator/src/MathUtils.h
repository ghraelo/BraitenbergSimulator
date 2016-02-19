#pragma once

#include <Box2D\Box2D.h>

class MathUtils
{
public:
	static float GetAngle(b2Vec2& a, b2Vec2& b);
	static bool  IsWithinCircle(b2Vec2& circleCenter, float circleRadius, b2Vec2& point);
	static b2Vec2 GetPointProjection(b2Vec2& lineA, b2Vec2& lineB, b2Vec2& point);
	static void GetIntersections(b2Vec2& aPos, b2Vec2& bPos, float lightRadius, b2Vec2& intersection1, b2Vec2& intersection2);
private:
	MathUtils();
};