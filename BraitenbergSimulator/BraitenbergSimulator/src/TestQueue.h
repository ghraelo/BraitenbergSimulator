#pragma once

#include <queue>
#include <vector>
#include <string>

#include "Scene.h"
#include "SimulationThread.h"

class TestQueue
{
public:
	TestQueue();
	void Begin(const std::vector<std::string>& fileNames);
	bool IsRunning();
	void Update();
	void End();
	SimulationThread* GetCurrentThread();
	int GetTestNo();
	bool IsFinished();
private:
	std::queue<ScenePtr> m_testQueue;
	SimulationThreadPtr m_currTest;
	bool m_next = true;
	int m_testNo = -1;
	bool m_finished = false;
};
