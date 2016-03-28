#pragma once

#include "Scene.h"

#include <memory>

#include <thread>
#include <mutex>
#include <atomic>

class SimulationThread
{
public:
	SimulationThread();
	SimulationThread(ScenePtr scene, int iterations);
	double GetElapsedTime(); //returns elapsed simulation time in seconds
	bool IsDone();
	void Exit();
protected:
	void ThreadMethod(ScenePtr& scene, int iterations);

	std::thread m_thread;
	std::mutex mtx;
	std::atomic<bool> m_threadShouldExit = false;
	std::atomic<bool> m_threadComplete = false;
	std::atomic<double> m_elapsedTime = 0.0;

};

typedef std::unique_ptr<SimulationThread> SimulationThreadPtr;