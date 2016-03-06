#pragma once

#include "ControlStrategy.h"
#include "CSL.h"

class SingleCSLStrategy : public ControlStrategy
{
public:
	SingleCSLStrategy();
	SingleCSLStrategy(float gi, float gf);
	void Update() override;
private:
	CSL m_CSL;
};
