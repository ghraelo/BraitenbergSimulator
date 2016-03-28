#include "SimulationThread.h"

#include "SimManager.h"
#include "DataRecorder.h"

#include <sstream>

SimulationThread::SimulationThread()
{
}

SimulationThread::SimulationThread(ScenePtr scene, int iterations)
{
	m_thread = std::thread(&SimulationThread::ThreadMethod, this, std::move(scene), iterations);
}

double SimulationThread::GetElapsedTime()
{
	return m_elapsedTime;
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

	DataRecorder dr;

	//open files for writing
	for (auto &obj : simManager.GetCurrentScene()->m_vehicles)
	{
		CSVRow headerRow;
		headerRow.m_cellData.push_back("events");
		headerRow.m_cellData.push_back("t");
		headerRow.m_cellData.push_back("x");
		headerRow.m_cellData.push_back("y");
		headerRow.m_cellData.push_back("angle");
		headerRow.m_cellData.push_back("distance travelled");
		dr.BeginFile(headerRow, obj->GetName());
	}

	//run simulation
	for (int i = 0; i < iterations; i++)
	{
		m_elapsedTime = m_elapsedTime + 1.0f / 60;

		//60 hz timestep
		simManager.Step(1.0f / 60);

		//record data
		for (auto &obj : simManager.GetCurrentScene()->m_vehicles)
		{
			CSVRow dataRow;
			std::stringstream conv;

			//events
			if (simManager.GetEventFlags() == EF_BoundaryCollision)
			{
				conv << "B";
			}
			dataRow.m_cellData.push_back(conv.str());
			conv.str(std::string());
			conv.clear();

			//time
			conv << m_elapsedTime;
			dataRow.m_cellData.push_back(conv.str());
			conv.str(std::string());
			conv.clear();

			//x
			conv << obj->GetPosition().x;
			dataRow.m_cellData.push_back(conv.str());
			conv.str(std::string());
			conv.clear();

			//y
			conv << obj->GetPosition().y;
			dataRow.m_cellData.push_back(conv.str());
			conv.str(std::string());
			conv.clear();

			//angle
			dataRow.m_cellData.push_back("");
			//distance travelled
			dataRow.m_cellData.push_back("");

			dr.Record(dataRow, obj->GetName());
		}

		if (m_threadShouldExit == true)
			break;
	}
	//close files
	dr.CloseAll();
	m_threadComplete = true;
	//printf("simulation done!\n");
}
