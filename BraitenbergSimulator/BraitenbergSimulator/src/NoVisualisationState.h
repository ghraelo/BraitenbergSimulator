#pragma once

#include "SimState.h"
#include "NoVisUISettings.h"
#include "UIManager.h"
#include "GUIRenderer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Boundary.h"
#include "SimManager.h"
#include "DataRecorder.h"
#include "SimulationThread.h"

#include <Box2D\Box2D.h>
#include <memory>

#include <thread>
#include <mutex>
#include <condition_variable>

//base UI settings (passed to/from UI manager)

typedef std::unique_ptr<b2World> b2WorldPtr;

class NoVisualisationState : public SimState
{
public:
	NoVisualisationState();
	void Init(SimEngine & se) override;
	void Cleanup() override;
	void Update(SimEngine& se) override;
	void Draw(SimEngine& se) override;
	void HandleEvents(SimEngine& se) override;
	void OnScroll(double scrollOffset) override;
private:
	void LoadScene(ScenePtr& ptr_scene);
	UIManager uim;
	NoVisUISettings m_baseSettings;
	GUIRenderer guiRenderer;
	b2WorldPtr world;
	ScenePtr m_currentScene;
	Renderer m_sceneRenderer;
	StatisticsManager sm;
	bool m_dragging = false;
	MouseState prevMouseState;
	BoundaryPtr worldBoundary;
	double simTime = 0.0;

	SimulationThreadPtr simThread;

};
