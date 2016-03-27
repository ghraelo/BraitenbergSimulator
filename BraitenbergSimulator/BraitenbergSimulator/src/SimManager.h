#pragma once

#include "Scene.h"
#include "Boundary.h"
#include "Renderer.h"
#include "EventFlags.h"

#include <memory>

typedef std::unique_ptr<b2World> b2WorldPtr;

class Renderer;

class SimManager : public IRenderable
{
public:
	SimManager();
	SimManager(ScenePtr& initialScene);
	Scene* GetCurrentScene();
	void LoadScene(ScenePtr& scene);
	void Step(float timeStep);
	void Render(NVGcontext* vg, Renderer& renderer) override;
	EventFlags GetEventFlags();
private:
	b2WorldPtr m_world;
	ScenePtr m_currentScene;
	BoundaryPtr m_boundary;
	std::vector<char> m_eventFlags;
};

typedef std::unique_ptr<SimManager> SimManagerPtr;