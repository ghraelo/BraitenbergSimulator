#include "SimState.h"
#include "SimEngine.h"

SimState::SimState()
{
}

void SimState::ChangeState(SimEngine & se, SimStatePtr & state)
{
	se.ChangeState(state);
}
