#pragma once

#include <memory>
#include <map>
#include <string>

typedef std::pair<std::string, float> InternalDataValue;

class ControlStrategy
{
public:
	ControlStrategy();
	void SetInputs(float left, float right);
	virtual void Update() = 0;
	void GetOutput(float& left, float& right);
	virtual std::map<std::string, float> GetInternalData();
protected:
	virtual void InitMap() = 0;
	float m_leftInput;
	float m_rightInput;
	float m_leftOutput;
	float m_rightOutput;
	std::map<std::string, float> m_internalData;
};

typedef std::unique_ptr<ControlStrategy> ControlStrategyPtr;