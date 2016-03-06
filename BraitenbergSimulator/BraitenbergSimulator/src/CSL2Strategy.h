#pragma once

#include "ControlStrategy.h"
#include "CSL2.h"

class CSL2Strategy : public ControlStrategy
{
public:
	CSL2Strategy();
	CSL2Strategy(float gi, float gf);
	void Update() override;
private:
	CSL2 m_leftCSL;
	CSL2 m_rightCSL;
};
