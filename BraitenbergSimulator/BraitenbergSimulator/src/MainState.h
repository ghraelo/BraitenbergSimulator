#pragma once

#include "SimState.h"
#include "BaseUISettings.h"
#include "UIManager.h"
#include "GUIRenderer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"

#include <Box2D\Box2D.h>
#include <memory>
//base UI settings (passed to/from UI manager)

typedef std::unique_ptr<b2World> b2WorldPtr;

class MainState : public SimState
{
public:
	MainState();
	void Init(SimEngine & se) override;
	void Cleanup() override;
	void Update(SimEngine& se) override;
	void Draw(SimEngine& se) override;
	void HandleEvents(SimEngine& se) override;
	void OnScroll(double scrollOffset) override;
private:
	void LoadScene(ScenePtr& ptr_scene);
	UIManager uim;
	BaseUISettings m_baseSettings;
	GUIRenderer guiRenderer;
	b2WorldPtr world;
	ScenePtr m_currentScene;
	Renderer m_sceneRenderer;
	StatisticsManager sm;
	CameraPtr cam;
	bool m_dragging = false;
	MouseState prevMouseState;
};
