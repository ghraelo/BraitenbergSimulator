#include "CSL2Strategy.h"

CSL2Strategy::CSL2Strategy()
	:ControlStrategy()
{
}

CSL2Strategy::CSL2Strategy(float gi, float gf)
	: ControlStrategy(), m_leftCSL(gf, gi), m_rightCSL(gf, gi)
{
}

void CSL2Strategy::Update()
{

	m_leftOutput = m_leftCSL.Update(m_leftInput * 2 - 1);
	m_rightOutput = m_rightCSL.Update(m_rightInput * 2 - 1);
}
