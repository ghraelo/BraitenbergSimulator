#include "ThreeBStrategy.h"

ThreeBStrategy::ThreeBStrategy()
	:ControlStrategy()
{
}

void ThreeBStrategy::Update()
{
	m_leftOutput = 5 * (1 - m_rightInput);
	m_rightOutput = 5 * (1 - m_leftInput);
}
