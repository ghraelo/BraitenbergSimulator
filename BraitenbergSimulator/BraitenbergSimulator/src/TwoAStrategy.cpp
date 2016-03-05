#include "TwoAStrategy.h"

TwoAStrategy::TwoAStrategy()
	:ControlStrategy()
{
}

void TwoAStrategy::Update()
{
	m_leftOutput = m_leftInput;
	m_rightOutput = m_rightInput;
}
