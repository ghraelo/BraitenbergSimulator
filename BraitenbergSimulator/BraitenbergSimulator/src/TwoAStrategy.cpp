#include "TwoAStrategy.h"

TwoAStrategy::TwoAStrategy()
	:ControlStrategy()
{
	InitMap();
}

void TwoAStrategy::Update()
{
	m_leftOutput = m_leftInput;

	m_internalData["leftInput"] = m_leftInput;
	m_internalData["leftOutput"] = m_leftOutput;

	m_rightOutput = m_rightInput;

	m_internalData["rightInput"] = m_rightInput;
	m_internalData["rightOutput"] = m_rightOutput;
}

void TwoAStrategy::InitMap()
{
	m_internalData.insert(InternalDataValue("rightInput", 0.0f));
	m_internalData.insert(InternalDataValue("leftInput", 0.0f));
	m_internalData.insert(InternalDataValue("rightOutput", 0.0f));
	m_internalData.insert(InternalDataValue("leftOutput", 0.0f));
}
