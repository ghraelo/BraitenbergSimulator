#pragma once
#include "ControlStrategy.h"

class TwoBStrategy : public ControlStrategy
{
public:
	TwoBStrategy();
	void Update() override;
};