#pragma once



enum EventFlags
{
	EF_None = 1,
	EF_BoundaryCollision = 2,
	EF_Oscillation = 4,
	EF_VehicleCollision = 8
};
//DEFINE_ENUM_FLAG_OPERATORS(EventFlags)