#include "CrossedCSLStrategy.h"

CrossedCSLStrategy::CrossedCSLStrategy()
	:ControlStrategy()
{
	InitMap();
}

CrossedCSLStrategy::CrossedCSLStrategy(float gi, float gf)
	: ControlStrategy(), m_leftCSL(gf, gi), m_rightCSL(gf, gi)
{
	InitMap();
}

void CrossedCSLStrategy::Update()
{
	m_leftOutput = m_leftCSL.Update(m_rightInput * 2 - 1);

	UpdateMap("left", m_leftCSL.GetInternalData());

	m_rightOutput = m_rightCSL.Update(m_leftInput * 2 - 1);

	UpdateMap("right", m_rightCSL.GetInternalData());
}

void CrossedCSLStrategy::InitMap()
{
	m_internalData.insert(InternalDataValue("rightInput", 0.0f));
	m_internalData.insert(InternalDataValue("leftInput", 0.0f));
	m_internalData.insert(InternalDataValue("rightDelay", 0.0f));
	m_internalData.insert(InternalDataValue("leftDelay", 0.0f));
	m_internalData.insert(InternalDataValue("rightOutput", 0.0f));
	m_internalData.insert(InternalDataValue("leftOutput", 0.0f));
}

void CrossedCSLStrategy::UpdateMap(std::string csl, CSLInternalData cid)
{
	m_internalData[csl + "Input"] = cid.m_input;
	m_internalData[csl + "Delay"] = cid.m_delay;
	m_internalData[csl + "Output"] = cid.m_output;
}
