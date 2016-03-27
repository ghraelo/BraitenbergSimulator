#include "ThreeAStrategy.h"

ThreeAStrategy::ThreeAStrategy()
	:ControlStrategy()
{
}

void ThreeAStrategy::Update()
{
	m_leftOutput = (1 - m_leftInput);
	m_rightOutput = (1 - m_rightInput);
}
