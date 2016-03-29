#pragma once

#include "ControlStrategy.h"
#include "CSL.h"

class CSLStrategy : public ControlStrategy
{
public:
	CSLStrategy();
	CSLStrategy(float gi, float gf);
	void Update() override;
	void InitMap() override;
private:
	void UpdateMap(std::string csl, CSLInternalData cid);
	CSL m_leftCSL;
	CSL m_rightCSL;
};