#pragma once

#include "ControlStrategy.h"
#include "CSL.h"

class CrossedCSLStrategy : public ControlStrategy
{
public:
	CrossedCSLStrategy();
	CrossedCSLStrategy(float gi, float gf);
	void Update() override;
	void InitMap() override;
	void UpdateMap(std::string csl, CSLInternalData cid);
private:
	CSL m_leftCSL;
	CSL m_rightCSL;
};
