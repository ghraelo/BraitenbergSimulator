#include "SingleCSLStrategy.h"

SingleCSLStrategy::SingleCSLStrategy()
	:ControlStrategy()
{
}

SingleCSLStrategy::SingleCSLStrategy(float gi, float gf)
	: ControlStrategy(), m_CSL(gf, gi)
{
}

void SingleCSLStrategy::Update()
{

	m_leftOutput = m_CSL.Update(m_leftInput);
	m_rightOutput = m_CSL.Update(m_rightInput);
}
