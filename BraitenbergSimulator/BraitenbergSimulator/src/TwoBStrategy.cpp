#include "TwoBStrategy.h"

TwoBStrategy::TwoBStrategy()
	:ControlStrategy()
{
	InitMap();
}

void TwoBStrategy::Update()
{
	m_leftOutput = m_rightInput;

	m_internalData["rightInput"] = m_rightInput;
	m_internalData["leftOutput"] = m_leftOutput;

	m_rightOutput = m_leftInput;

	m_internalData["leftInput"] = m_leftInput;
	m_internalData["rightOutput"] = m_rightOutput;


}

void TwoBStrategy::InitMap()
{
	m_internalData.insert(InternalDataValue("rightInput", 0.0f));
	m_internalData.insert(InternalDataValue("leftInput", 0.0f));
	m_internalData.insert(InternalDataValue("rightOutput", 0.0f));
	m_internalData.insert(InternalDataValue("leftOutput", 0.0f));
}
