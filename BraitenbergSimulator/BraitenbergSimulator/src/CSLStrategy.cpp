#include "CSLStrategy.h"

CSLStrategy::CSLStrategy()
	:ControlStrategy()
{
}

CSLStrategy::CSLStrategy(float gi, float gf)
	:ControlStrategy(),m_leftCSL(gf,gi),m_rightCSL(gf,gi)
{
}

void CSLStrategy::Update()
{
	m_leftOutput = m_leftCSL.Update(m_leftInput);
	m_rightOutput = m_rightCSL.Update(m_rightInput);
}