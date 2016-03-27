#pragma once
#include "ControlStrategy.h"

class ThreeAStrategy : public ControlStrategy
{
public:
	ThreeAStrategy();
	void Update() override;
};