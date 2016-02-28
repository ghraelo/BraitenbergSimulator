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
		GetLightBoundary(ls.GetPosition(), ls.GetRadius(), rlp);
		Interval temp(rlp.m_point1, rlp.m_point2);
		intervals.push_back(temp);
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



void LightSensor::GetLightBoundary(b2Vec2& lightPos, float lightRadius, relLightPos& relativeLightBoundary)
{
	float light_sensor_dist = b2Distance(this->GetPosition(), lightPos);
	
	//bounds tests
	b2Vec2 right_bound = GetArcEnd(light_sensor_dist, false);
	b2Vec2 left_bound = GetArcEnd(light_sensor_dist, true);
	b2Vec2 sensor_dir = GetParentBody()->GetWorldVector(m_direction_vector);
	b2Vec2 pos = this->GetPosition();
	b2Vec2 i1, i2;
	MathUtils::GetIntersections(this->GetPosition(), lightPos, lightRadius, i1, i2);


	//char text_buffer[50] = { 0 };
	//sprintf(text_buffer,"left angle: %5.2f, right angle: %5.2f\n", GetAngle(i2 - pos, sensor_dir) * (180/M_PI), GetAngle(i1-pos, sensor_dir) * (180 / M_PI));
	//OutputDebugString(text_buffer);

	bool withinLeft = MathUtils::IsWithinCircle(lightPos, lightRadius, left_bound);
	bool withinRight = MathUtils::IsWithinCircle(lightPos, lightRadius, right_bound);

	float scaleFactor = 1/b2Distance(left_bound, right_bound);

	relativeLightBoundary.m_point1 = 0;
	relativeLightBoundary.m_point2 = 0;

	if (withinLeft && withinRight)
	{
		//OutputDebugString("both bounds inside circle\n");
		relativeLightBoundary.m_point1 = 0;
		relativeLightBoundary.m_point2 = 1.0f;
		return;
	}
	else
	{

		if (MathUtils::GetAngle(i2 - pos, sensor_dir) < m_aperture_angle / 2)
		{
			relativeLightBoundary.m_point1 = b2Distance(left_bound,MathUtils::GetPointProjection(left_bound, right_bound, i2)) * scaleFactor;
			//OutputDebugString("left intersection pt within sensor cone\n");
		}

		if (MathUtils::GetAngle(i1 - pos, sensor_dir) < m_aperture_angle / 2)
		{
			relativeLightBoundary.m_point2 = b2Distance(left_bound,MathUtils::GetPointProjection(left_bound, right_bound, i1)) * scaleFactor;
			//OutputDebugString("right intersection pt within sensor cone\n");
		}

		if (withinLeft)
		{
			//OutputDebugString("left bound inside circle\n");
			relativeLightBoundary.m_point1 = 0;
		}
		else if (withinRight)
		{
			//OutputDebugString("right bound inside circle\n");
			relativeLightBoundary.m_point2 = 1.0f;
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
