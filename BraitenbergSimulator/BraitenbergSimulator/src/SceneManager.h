#pragma once

#include <vector>
#include <memory>

#include <Box2D\Box2D.h>
#include <boost\circular_buffer.hpp>

#include "SimObject.h"
#include "Renderer.h"
#include "UIManager.h"
#include "Scene.h"
#include "DataRecorder.h"

#include "StatisticsManager.h"

typedef std::unique_ptr<b2World> WorldPtr;

class SceneManager
{
public:
	SceneManager();
	SceneManager(WorldPtr world);

	void LoadScene(ScenePtr& ptr_scene);
	void Render();
	void BindPhysics();
	void Step();
	void Sample();

	const StatisticsManager& GetStatsMan() const;

private:
	bool m_sceneLoaded = false;
	int m_tsCount = 0;
	long int sampleCount = 0;
	DataRecorder m_dataRecorder;
	Renderer m_sceneRenderer;
	StatisticsManager m_statsManager;
	WorldPtr m_world;
	ScenePtr m_currentScene;
	boost::circular_buffer<float> circ;
};