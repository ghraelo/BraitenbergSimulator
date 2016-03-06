#include "CrossedCSLStrategy.h"

CrossedCSLStrategy::CrossedCSLStrategy()
	:ControlStrategy()
{
}

CrossedCSLStrategy::CrossedCSLStrategy(float gi, float gf)
	: ControlStrategy(), m_leftCSL(gf, gi), m_rightCSL(gf, gi)
{
}

void CrossedCSLStrategy::Update()
{
	m_leftOutput = m_leftCSL.Update(m_rightInput);
	m_rightOutput = m_rightCSL.Update(m_leftInput);
}
