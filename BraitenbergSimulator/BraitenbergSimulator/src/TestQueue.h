#pragma once

#include <queue>
#include <string>
#include <vector>

#include "Scene.h"

class TestQueue
{
public:
	TestQueue();
	void Push(std::string fileName);
	ScenePtr Pop();

private:
	std::queue<ScenePtr> m_sceneQueue;
};