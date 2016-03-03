#pragma once

#include <vector>
#include <memory>

#include <Box2D\Box2D.h>

#include "SimObject.h"
#include "Renderer.h"
#include "UIManager.h"
#include "Scene.h"
#include "DataRecorder.h"

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
private:
	bool m_sceneLoaded = false;
	int m_tsCount = 0;
	long int sampleCount = 0;
	DataRecorder m_dataRecorder;
	Renderer m_sceneRenderer;
	WorldPtr m_world;
	ScenePtr m_currentScene;
};