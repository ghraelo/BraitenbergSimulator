#pragma once

#include <vector>
#include <memory>

#include <Box2D\Box2D.h>

#include "SimObject.h"
#include "Renderer.h"
#include "UIManager.h"
#include "Scene.h"

typedef std::unique_ptr<b2World> WorldPtr;

class Simulator
{
public:
	Simulator();
	Simulator(WorldPtr world);

	void LoadScene(ScenePtr& ptr_scene);
	void Render();
	void BindPhysics();
	void Step();
	void RayCast(b2Vec2 point);
private:
	bool m_sceneLoaded = false;
	Renderer m_sceneRenderer;
	WorldPtr m_world;
	ScenePtr m_currentScene;
	std::vector<b2Vec2> rayCastPoly;
};