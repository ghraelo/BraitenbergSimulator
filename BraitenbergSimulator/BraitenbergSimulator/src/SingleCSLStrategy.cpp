#include "SingleCSLStrategy.h"

SingleCSLStrategy::SingleCSLStrategy()
	:ControlStrategy()
{
	InitMap();
}

SingleCSLStrategy::SingleCSLStrategy(float gi, float gf)
	: ControlStrategy(), m_CSL(gf, gi)
{
	InitMap();
}

void SingleCSLStrategy::Update()
{

	m_leftOutput = m_CSL.Update(m_leftInput * 2 - 1);

	CSLInternalData first = m_CSL.GetInternalData();
	m_internalData["firstInput"] = first.m_input;
	m_internalData["firstDelay"] = first.m_output;
	m_internalData["firstOutput"] = first.m_delay;

	m_rightOutput = m_CSL.Update(m_rightInput * 2 - 1);

	CSLInternalData second = m_CSL.GetInternalData();
	m_internalData["secondInput"] = second.m_input;
	m_internalData["secondDelay"] = second.m_output;
	m_internalData["secondOutput"] = second.m_delay;
}

void SingleCSLStrategy::InitMap()
{
	m_internalData.insert(InternalDataValue("firstInput", 0.0f));
	m_internalData.insert(InternalDataValue("secondInput", 0.0f));
	m_internalData.insert(InternalDataValue("firstDelay", 0.0f));
	m_internalData.insert(InternalDataValue("secondDelay", 0.0f));
	m_internalData.insert(InternalDataValue("firstOutput", 0.0f));
	m_internalData.insert(InternalDataValue("secondOutput", 0.0f));
}