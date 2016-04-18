#pragma once

#include <Box2D\Box2D.h>
#include "SimObjectInfo.h"
#include "VehicleMonitor.h"

class VehicleContactListener : public b2ContactListener
{
public:
	VehicleContactListener(VehicleMonitor* vehicleMonitor)
		:m_monitor(vehicleMonitor)
	{

	};

	void BeginContact(b2Contact* contact)
	{
		if (GetPointerFromFixture(contact->GetFixtureA) == m_monitor->GetVehiclePointer() || GetPointerFromFixture(contact->GetFixtureB) == m_monitor->GetVehiclePointer())
		{
			m_monitor->SetIsColliding(true);
		}
	};

	void EndContact(b2Contact* contact)
	{
		if (GetPointerFromFixture(contact->GetFixtureA) == m_monitor->GetVehiclePointer() || GetPointerFromFixture(contact->GetFixtureB) == m_monitor->GetVehiclePointer())
		{
			m_monitor->SetIsColliding(false);
		}
	};

protected:
	VehicleMonitor* m_monitor;
	SimObject* GetPointerFromFixture(b2Fixture* fixture)
	{
		b2Body* body = fixture->GetBody();

		SimObjectInfo* info = (SimObjectInfo*)(body->GetUserData());
		return (info->m_obj);
	};
};