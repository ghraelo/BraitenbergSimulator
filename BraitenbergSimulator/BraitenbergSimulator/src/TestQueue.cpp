#include "TestQueue.h"

#include "ResourceManager.h"

TestQueue::TestQueue()
{
}

void TestQueue::Begin(const std::vector<std::string>& fileNames)
{
	for (auto& file : fileNames)
	{
		m_testQueue.push(ResourceManager::LoadScene(file));
	}
}

bool TestQueue::IsRunning()
{
	return m_currTest ? !m_currTest->IsDone() : false;
}

void TestQueue::Update()
{
	if (!m_currTest)
	{
		if (!m_testQueue.empty())
		{
			//create sim thread from scene at front of queue
			m_currTest = std::make_unique<SimulationThread>(std::move(m_testQueue.front()), 60000);
			m_testQueue.pop(); //pop empty ptr off queue
			m_testNo++;
		}
	}
	else
	{
		if (IsRunning())
		{
			return; //do nothing if thread is running
		}
		else
		{
			m_currTest->Exit(); //NEED THIS TO NOT CRASH
			m_currTest.reset();
			if (m_testQueue.empty())
			{
				m_finished = true;
				m_testNo++;
			}
		}
	}

}

void TestQueue::End()
{
	if (m_currTest)
		m_currTest->Exit();
}

SimulationThread * TestQueue::GetCurrentThread()
{
	return m_currTest.get();
}

int TestQueue::GetTestNo()
{
	return m_testNo;
}

bool TestQueue::IsFinished()
{
	return m_finished;
}
