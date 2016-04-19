#include "VehicleContactListener.h"
#include "VehicleMonitor.h"

VehicleContactListener::VehicleContactListener(VehicleMonitor * vehicleMonitor)
	:m_monitor(vehicleMonitor)
{

}

void VehicleContactListener::BeginContact(b2Contact * contact)
{
	if (GetPointerFromFixture(contact->GetFixtureA()) == m_monitor->GetPointer())
	{
		m_monitor->SetIsColliding(true);
		m_monitor->SetObstacleName(GetPointerFromFixture(contact->GetFixtureB())->GetName());
	}
	else if (GetPointerFromFixture(contact->GetFixtureB()) == m_monitor->GetPointer())
	{
		m_monitor->SetIsColliding(true);
		m_monitor->SetObstacleName(GetPointerFromFixture(contact->GetFixtureB())->GetName());
	}
}

void VehicleContactListener::EndContact(b2Contact * contact)
{
	if (GetPointerFromFixture(contact->GetFixtureA()) == m_monitor->GetPointer() || GetPointerFromFixture(contact->GetFixtureB()) == m_monitor->GetPointer())
	{
		m_monitor->SetIsColliding(false);
	}
}

SimObject * VehicleContactListener::GetPointerFromFixture(b2Fixture * fixture)
{
	b2Body* body = fixture->GetBody();

	SimObjectInfo* info = (SimObjectInfo*)(body->GetUserData());
	return (info->m_obj);
}


