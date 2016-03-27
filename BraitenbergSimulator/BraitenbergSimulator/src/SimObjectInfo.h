#pragma once

#include "SimObject.h"
#include <string>
#include <memory>

struct SimObjectInfo
{
	std::string m_type;
	SimObject* m_obj;
};

typedef std::unique_ptr<SimObjectInfo> SimObjectInfoPtr;