#pragma once

#include "SimState.h"

class MainState : public SimState
{
public:
	MainState();
	void Init() override;
	void Cleanup() override;
	void Update(SimEngine& se) override;
	void Draw(SimEngine& se) override;
private:
};