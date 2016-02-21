#pragma once

#include "SimObject.h"
#include <string>

struct SimObjectInfo
{
	std::string m_type;
	SimObject* m_obj;
};