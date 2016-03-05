#pragma once

#include "ControlStrategy.h"
#include "CSL.h"

class CSLStrategy : public ControlStrategy
{
public:
	CSLStrategy();
	CSLStrategy(float gi, float gf);
	void Update() override;
private:
	CSL m_leftCSL;
	CSL m_rightCSL;
};