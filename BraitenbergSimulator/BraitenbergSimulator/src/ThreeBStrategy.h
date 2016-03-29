#pragma once
#include "ControlStrategy.h"

class ThreeBStrategy : public ControlStrategy
{
public:
	ThreeBStrategy();
	void Update() override;
	void InitMap() override;
};
