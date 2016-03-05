#pragma once
#include "ControlStrategy.h"

class TwoAStrategy : public ControlStrategy
{
public:
	TwoAStrategy();
	void Update() override;
};