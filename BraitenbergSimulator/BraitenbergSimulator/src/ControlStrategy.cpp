#include "ControlStrategy.h"

ControlStrategy::ControlStrategy()
	:m_leftInput(0),m_rightInput(0),m_leftOutput(0),m_rightOutput(0)
{
}

void ControlStrategy::SetInputs(float left, float right)
{
	m_leftInput = left;
	m_rightInput = right;
}

void ControlStrategy::GetOutput(float & left, float & right)
{
	left = m_leftOutput;
	right = m_rightOutput;
}
