#pragma once
#include "Testbed\Braitenberg\Vehicle.h"
#include "Testbed\Framework\DebugDraw.h"

class VehicleRenderer
{
public:
	VehicleRenderer(Camera& camera);
	void Render(Vehicle v);

private:
	Camera& m_camera;

	void DrawSensorCone(LightSensor sensor, float dist);
	void BeginRender();
	void EndRender();
};