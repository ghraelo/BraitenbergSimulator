#include "TestQueue.h"
#include "ResourceManager.h"

TestQueue::TestQueue()
{
}

void TestQueue::Push(std::string fileName)
{
	m_sceneQueue.push(ResourceManager::LoadScene(fileName));
}

ScenePtr TestQueue::Pop()
{
	ScenePtr out = std::move(m_sceneQueue.front());
	m_sceneQueue.pop();

	return out;
}
