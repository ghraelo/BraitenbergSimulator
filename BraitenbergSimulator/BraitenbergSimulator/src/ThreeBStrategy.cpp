#include "ThreeBStrategy.h"

ThreeBStrategy::ThreeBStrategy()
	:ControlStrategy()
{
	InitMap();
}

void ThreeBStrategy::Update()
{
	m_leftOutput = 5 * (1 - m_rightInput);

	m_internalData["rightInput"] = m_rightInput;
	m_internalData["leftOutput"] = m_leftOutput;

	m_rightOutput = 5 * (1 - m_leftInput);

	m_internalData["leftInput"] = m_leftInput;
	m_internalData["rightOutput"] = m_rightOutput;
}

void ThreeBStrategy::InitMap()
{
	m_internalData.insert(InternalDataValue("rightInput", 0.0f));
	m_internalData.insert(InternalDataValue("leftInput", 0.0f));
	m_internalData.insert(InternalDataValue("rightOutput", 0.0f));
	m_internalData.insert(InternalDataValue("leftOutput", 0.0f));
}
