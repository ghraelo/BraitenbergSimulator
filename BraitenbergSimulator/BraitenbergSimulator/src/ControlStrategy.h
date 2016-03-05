#pragma once

#include <memory>

class ControlStrategy
{
public:
	ControlStrategy();
	void SetInputs(float left, float right);
	virtual void Update() = 0;
	void GetOutput(float& left, float& right);
protected:
	float m_leftInput;
	float m_rightInput;
	float m_leftOutput;
	float m_rightOutput;
};

typedef std::unique_ptr<ControlStrategy> ControlStrategyPtr;