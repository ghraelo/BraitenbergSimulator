#include "LightSensor.h"

#define M_PI 3.14159265

#include "Renderer.h"
#include "MathUtils.h"
#include "Vehicle.h"

#include <stack>
#include <algorithm>
#include <iostream>

LightSensor::LightSensor(Vehicle * parent, sensorInfo info)
	:m_parent(parent), m_offset(info.m_offset), m_aperture_angle(info.m_aperture), m_direction_vector(info.m_direction)
{
}

LightSensor::LightSensor(Vehicle * parent)
	:m_parent(parent), m_offset(b2Vec2_zero), m_aperture_angle(1.0f),m_direction_vector(-0.5,1)
{

}


LightSensor::LightSensor(Vehicle * parent, b2Vec2 offset, float aperture, b2Vec2 direction)
	:m_parent(parent), m_offset(offset), m_aperture_angle(aperture), m_direction_vector(direction)
{
}

void LightSensor::Render(Renderer& r)
{
	r.RenderLightSensor(*this);
}

b2Vec2 LightSensor::GetPosition()
{
	//x1 = x* cos(r) - y*sin(r)
	//y1 = x* sin(r) + y*cos(r)

	float x1 = m_offset.x * cos(GetParentBody()->GetAngle()) - m_offset.y * sin(GetParentBody()->GetAngle());
	float y1 = m_offset.x * sin(GetParentBody()->GetAngle()) + m_offset.y * cos(GetParentBody()->GetAngle());

	b2Vec2 new_offset(x1, y1);

	return GetParentBody()->GetPosition() + new_offset;

}

/**
1. Sort the intervals based on increasing order of
starting time.
2. Push the first interval on to a stack.
3. For each interval do the following
a. If the current interval does not overlap with the stack
top, push it.
b. If the current interval overlaps with stack top and ending
time of current interval is more than that of stack top,
update stack top with the ending  time of current interval.
4. At the end stack contains the merged intervals.
http://www.geeksforgeeks.org/merging-intervals/
**/



bool compareIntervals(Interval a, Interval b)
{
	return a.first < b.first;
}


float LightSensor::GetLight(std::vector<LightSource>& lightSources)
{
	b2World* world = m_parent->m_body->GetWorld();
	Raycaster r(world, GetPosition(), Rectangle(g_camera.GetCorner(CC_TOP_RIGHT), g_camera.GetCorner(CC_BOTTOM_LEFT)));
	r.AddIgnoreBody(m_parent->m_body);
	b2Vec2 right_bound = GetArcEnd(10.0f, false);
	b2Vec2 left_bound = GetArcEnd(10.0f, true);

	float angle1 = atan2((left_bound - GetPosition()).y, (left_bound - GetPosition()).x);
	float angle2 = atan2((right_bound - GetPosition()).y, (right_bound - GetPosition()).x);

	r.Cast(angle1,angle2);

	std::vector<b2Vec2> rcp = r.GetRayCastPoly();

	g_debugDraw.DrawConcavePolygon(rcp, b2Color(1, 1, 1, 0.5));


	m_intervals.clear();

	std::vector<Interval> intervals;

	//build vector of intervals
	for (auto& ls : lightSources)
	{
		relLightPos rlp;
		std::vector<Interval> temp;

		GetLightBoundary(ls.GetPosition(), ls.GetRadius(), temp, rcp);
		intervals.insert(intervals.end(),temp.begin(),temp.end());
	}

	if (intervals.size() <= 0)
		return 0;

	//sort ascending
	std::sort(intervals.begin(), intervals.end(), compareIntervals);

	std::stack<Interval> theStack;

	theStack.push(intervals[0]);

	for (unsigned int i = 1; i < intervals.size(); i++)
	{
		Interval* curr = &intervals[i];
		//get stack top
		Interval top = theStack.top();
		
		//push current interval if it doesn't overlap
		if (top.second < curr->first)
		{
			theStack.push(*curr);
		}
		else if (top.second < curr->second)
		{
			//update top if overlap
			top.second = curr->second;
			theStack.pop();
			theStack.push(top);
		}
	}
	float acc = 0.0f;
	while (!theStack.empty())
	{
		Interval t = theStack.top();
		acc += t.second - t.first;
		m_intervals.push_back(t);
		//std::cout << "[" << t.first << "," << t.second << "] ";
		theStack.pop();
	}
	//std::cout << "\r";

	return acc;
}



void LightSensor::GetLightBoundary(b2Vec2& lightPos, float lightRadius, std::vector<Interval>& intervals, std::vector<b2Vec2>& rayCastPoly)
{
	float light_sensor_dist = b2Distance(this->GetPosition(), lightPos);
	//bounds tests
	b2Vec2 right_bound = GetArcEnd(light_sensor_dist, false);
	b2Vec2 left_bound = GetArcEnd(light_sensor_dist, true);
	b2Vec2 sensor_dir = GetParentBody()->GetWorldVector(m_direction_vector);

	float angle1 = atan2((left_bound - GetPosition()).y, (left_bound - GetPosition()).x);
	float angle2 = atan2((right_bound - GetPosition()).y, (right_bound - GetPosition()).x);

	b2Vec2 normal(-sensor_dir.y, sensor_dir.x);
	normal.Normalize();

	b2Vec2 pos1 = lightPos + lightRadius * normal;
	b2Vec2 pos2 = lightPos - lightRadius * normal;

	if (MathUtils::PointInPoly(rayCastPoly, pos1))
		g_debugDraw.DrawPoint(pos1, 5, b2Color(1, 0, 0));
	else
		g_debugDraw.DrawPoint(pos1, 5, b2Color(0, 1, 0)); 
	if (MathUtils::PointInPoly(rayCastPoly, pos2))
		g_debugDraw.DrawPoint(pos2, 5, b2Color(1, 0, 0));
	else
		g_debugDraw.DrawPoint(pos2, 5, b2Color(0, 1, 0));

	std::vector<b2Vec2> intersections;

	g_debugDraw.DrawPolygon(&rayCastPoly[0], rayCastPoly.size(), b2Color(1, 0, 1));

	b2Vec2 p1 = rayCastPoly[rayCastPoly.size() - 1];
	//find intersections of circle diameter with polygon
	for (int32 i = 0; i < rayCastPoly.size(); ++i)
	{
		b2Vec2 p2 = rayCastPoly[i];

		b2Vec2 result;

		if (MathUtils::GetIntersectionLines(pos1, pos2, p1, p2, result) == 0)
		{
			g_debugDraw.DrawPoint(result, 5, b2Color(0, 0, 1));
			intersections.push_back(result);
		}

		p1 = p2;
	}

	if (MathUtils::PointInPoly(rayCastPoly, pos1))
	{
		intersections.push_back(pos1);
	}

	if (MathUtils::PointInPoly(rayCastPoly, pos2))
	{
		intersections.push_back(pos2);
	}

	//sort by angle
	std::sort(intersections.begin(), intersections.end(),
		[&](b2Vec2 a, b2Vec2 b) {
		float a_angle = atan2(a.y - GetPosition().y, a.x - GetPosition().x);
		float b_angle = atan2(b.y - GetPosition().y, b.x - GetPosition().x);
		if (a_angle == b_angle)
			return b2Distance(a, GetPosition()) > b2Distance(b, GetPosition());
		else
			return a_angle < b_angle;
	}
	);

	printf("size: %d\n", intersections.size());
	if (intersections.size() > 1)
	{
		b2Vec2 i1 = intersections[0];
		bool open = true;
		for (int i = 1; i < intersections.size(); ++i)
		{
			b2Vec2 i2 = intersections[i];
			if (open)
			{
				g_debugDraw.DrawSegment(i1, i2, b2Color(0, 1, 1));
			}
			open = !open;
			i1 = i2;
		}
	}
}


b2Vec2 LightSensor::GetArcEnd(float radius, bool positive)
{
	//x' = xcos(a) - ysin(a)
	//y' = xsin(a) + ycos(a)

	b2Vec2 dir = GetParentBody()->GetWorldVector(m_direction_vector);
	dir.Normalize(); //normalised direction vector of sensor

	b2Rot rotation;
	if(positive)
		rotation = b2Rot(m_aperture_angle/2);
	else
		rotation = b2Rot(-m_aperture_angle / 2);


	b2Vec2 endDir = b2Mul(rotation, dir);

	return this->GetPosition() + radius*endDir;
}

sensorInfo LightSensor::GetSensorInfo() const
{
	sensorInfo s;
	s.m_aperture = m_aperture_angle;
	s.m_direction = m_direction_vector;
	s.m_offset = m_offset;

	return s;
}

b2Body* LightSensor::GetParentBody()
{
	return m_parent->m_body;
}
