#pragma once

#include "SimState.h"
#include "NoVisUISettings.h"
#include "NoVisualisationUI.h"
#include "GUIRenderer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Boundary.h"
#include "SimManager.h"

#include "SimulationThread.h"
#include "TestQueue.h"

#include <Box2D\Box2D.h>
#include <memory>
#include <queue>

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
	NoVisualisationUI uim;
	NoVisUISettings m_baseSettings;
	GUIRenderer guiRenderer;
	Renderer m_sceneRenderer;
	MouseState prevMouseState;
	double simTime = 0.0;

	std::string m_currentSceneFileName;
	bool exiting = false;
	TestQueue testQueue;

};
