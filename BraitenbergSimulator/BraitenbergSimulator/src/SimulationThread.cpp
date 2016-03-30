#include "SimulationThread.h"

#include "SimManager.h"
#include "DataRecorder.h"
#include "MonitorManager.h"

#include <sstream>

SimulationThread::SimulationThread()
{
}

SimulationThread::SimulationThread(ScenePtr scene, int iterations)
	:m_iterations(iterations)
{
	m_thread = std::thread(&SimulationThread::ThreadMethod, this, std::move(scene), iterations);
}

double SimulationThread::GetElapsedTime()
{
	return m_elapsedTime;
}

double SimulationThread::GetMaxTime()
{
	return m_iterations * m_timeStep;
}

bool SimulationThread::IsDone()
{
	return m_threadComplete;
}

void SimulationThread::Exit()
{
	//tell thread to exit
	m_threadShouldExit = true;

	//block until exit
	m_thread.join();
}

void SimulationThread::ThreadMethod(ScenePtr& scene, int iterations)
{
	//create sim manager and data recorder
	SimManager simManager(scene);
	MonitorManager monitorManager("logs");
	
	monitorManager.Init(simManager.GetCurrentScene());

	BoundaryCallbackFunc func = [&](BoundaryType boundary, b2Vec2 coords, double time, Vehicle* vehicle) {
		monitorManager.AddBoundaryCollision(boundary, coords, time, vehicle);
	};

	simManager.GetBoundary()->RegisterCallback(func);

	//run simulation
	for (int i = 0; i < iterations; i++)
	{
		m_elapsedTime = m_elapsedTime + m_timeStep;

		//60 hz timestep
		simManager.Step(m_timeStep);

		//write csv
		monitorManager.RecordCSV();

		if (m_threadShouldExit == true)
			break;
	}
	monitorManager.SaveYAML();
	monitorManager.Close();
	m_threadComplete = true;
	printf("simulation done!\n");
}
