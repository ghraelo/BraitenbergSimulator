#include "LightSensor.h"

#define M_PI 3.14159265

#include <Windows.h>
#include "Renderer.h"
#include "MathUtils.h"

LightSensor::LightSensor()
{

}

LightSensor::LightSensor(b2Body * parent, sensorInfo info)
	:m_parent(parent), m_offset(info.m_offset), m_aperture_angle(info.m_aperture), m_direction_vector(info.m_direction)
{
}

LightSensor::LightSensor(b2Body* parent)
	:m_parent(parent), m_offset(b2Vec2_zero), m_aperture_angle(1.0f),m_direction_vector(-0.5,1)
{

}

LightSensor::LightSensor(b2Body* parent, b2Vec2 offset, float aperture, b2Vec2 direction)
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

	float x1 = m_offset.x * cos(m_parent->GetAngle()) - m_offset.y * sin(m_parent->GetAngle());
	float y1 = m_offset.x * sin(m_parent->GetAngle()) + m_offset.y * cos(m_parent->GetAngle());

	b2Vec2 new_offset(x1, y1);

	return m_parent->GetPosition() + new_offset;

}



void LightSensor::GetLightBoundary(b2Vec2& lightPos, float lightRadius, relLightPos& relativeLightBoundary)
{
	float light_sensor_dist = b2Distance(this->GetPosition(), lightPos);
	
	//bounds tests
	b2Vec2 right_bound = GetArcEnd(light_sensor_dist, false);
	b2Vec2 left_bound = GetArcEnd(light_sensor_dist, true);
	b2Vec2 sensor_dir = m_parent->GetWorldVector(m_direction_vector);
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

	b2Vec2 dir = m_parent->GetWorldVector(m_direction_vector);
	dir.Normalize(); //normalised direction vector of sensor

	b2Rot rotation;
	if(positive)
		rotation = b2Rot(m_aperture_angle/2);
	else
		rotation = b2Rot(-m_aperture_angle / 2);


	b2Vec2 endDir = b2Mul(rotation, dir);

	return this->GetPosition() + radius*endDir;
}
