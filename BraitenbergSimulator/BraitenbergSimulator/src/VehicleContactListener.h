#pragma once

#include <Box2D\Box2D.h>
#include "SimObjectInfo.h"

//forward declaration
class VehicleMonitor;

class VehicleContactListener : public b2ContactListener
{
public:
	VehicleContactListener(VehicleMonitor* vehicleMonitor);

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);

protected:
	VehicleMonitor* m_monitor;
	SimObject* GetPointerFromFixture(b2Fixture* fixture);
};