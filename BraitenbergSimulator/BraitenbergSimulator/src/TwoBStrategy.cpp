#include "TwoBStrategy.h"

TwoBStrategy::TwoBStrategy()
	:ControlStrategy()
{
}

void TwoBStrategy::Update()
{
	m_leftOutput = m_rightInput;
	m_rightOutput = m_leftInput;
}
