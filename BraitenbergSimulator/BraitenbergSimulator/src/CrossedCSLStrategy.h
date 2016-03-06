#pragma once

#include "ControlStrategy.h"
#include "CSL.h"

class CrossedCSLStrategy : public ControlStrategy
{
public:
	CrossedCSLStrategy();
	CrossedCSLStrategy(float gi, float gf);
	void Update() override;
private:
	CSL m_leftCSL;
	CSL m_rightCSL;
};
