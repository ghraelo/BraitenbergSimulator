#pragma once

#include "ControlStrategy.h"
#include "CSL2.h"

class CSL2Strategy : public ControlStrategy
{
public:
	CSL2Strategy();
	CSL2Strategy(float gi, float gf);
	void Update() override;
	void InitMap() override;
private:
	void UpdateMap(std::string csl, CSLInternalData cid);
	CSL2 m_leftCSL;
	CSL2 m_rightCSL;
};
