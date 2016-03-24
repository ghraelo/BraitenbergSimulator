#include "NoVisualisationState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"
#include "MathUtils.h"
#include "Statistics.h"

NoVisualisationState::NoVisualisationState()
	:sm(30)
{
}

void NoVisualisationState::Init(SimEngine & se)
{
	//set default scene file
	std::vector<std::string> out;
	ResourceManager::GetFilesInDirectory(out, "yaml");
	if (out.size() > 0)
	{
		m_baseSettings.activeSceneFilename = out[0];
	}
	else
	{
		printf("error: no scene files found\n");
	}

	//create world
	world = std::make_unique<b2World>(b2Vec2(0, 0));


	//load scene and bind physics
	LoadScene(ResourceManager::LoadScene(m_baseSettings.activeSceneFilename));

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->BindPhysics(world.get());
	}

	for (auto &obj : m_currentScene->m_vehicles)
	{
		std::function<float()> angleCallback = [&]() {
			return atan2(obj->m_body->GetWorldVector(b2Vec2(0, 1)).y, obj->m_body->GetWorldVector(b2Vec2(0, 1)).x);
		};

		std::function<float()> distCallback = [&]() {
			return b2Distance(obj->m_body->GetPosition(), m_currentScene->m_lights[0].GetPosition());
		};

		//add statistics watchers
		StatisticPtr s = std::make_unique<FixedWatcher>(obj->GetName() + " - angle", 10, 180,
			angleCallback, -M_PI, M_PI);

		StatisticPtr s2 = std::make_unique<Watcher>(obj->GetName() + " - distance", 10, 180,
			distCallback);

		StatisticPtr p1 = std::make_unique<AutocorrelationProcessor>(obj->GetName() + " - acor(distance)", dynamic_cast<Watcher*>(s2.get()));

		StatisticPtr p2 = std::make_unique<AutocorrelationProcessor>(obj->GetName() + " - acor(angle)", dynamic_cast<FixedWatcher*>(s.get()));

		StatisticPtr p3 = std::make_unique<PeriodicityDetectionProcessor>(obj->GetName() + " - dct(angle)", dynamic_cast<Watcher*>(s.get()));

		sm.AddStat(std::move(s));
		sm.AddStat(std::move(s2));
		sm.AddStat(std::move(p1));
		sm.AddStat(std::move(p2));
		sm.AddStat(std::move(p3));
	}

	worldBoundary = std::make_unique<Boundary>(world.get(), b2Vec2(-150.0f, 150.0f), 300.0f, 300.0f, 50.0f);

	prevMouseState = se.GetMouseState();
}

void NoVisualisationState::Cleanup()
{
}

void NoVisualisationState::Update(SimEngine & se)
{
	m_baseSettings.frameTime = se.GetFrameTime();
	if (m_baseSettings.shouldExit)
	{
		SimStatePtr p = std::make_unique<MenuState>();
		ChangeState(se, p);
		return;
	}

	world->Step(1.0f / 60, 8, 3);
	;	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Update(m_currentScene->m_lights, worldBoundary->GetRect());
	}

	worldBoundary->Update();

	sm.Update();
}

void NoVisualisationState::Draw(SimEngine & se)
{
	NVGcontext* vg = se.GetContext();

	// draw interface here
	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
	uim.DrawBaseUI(m_baseSettings, se.GetWindowState());

	if (m_baseSettings.showStatsPane)
		uim.DrawStatsPane(sm, se.GetWindowState());

	imguiEndFrame();

	guiRenderer.Flush(vg);
}

void NoVisualisationState::HandleEvents(SimEngine & se)
{
}

void NoVisualisationState::OnScroll(double scrollOffset)
{
}

void NoVisualisationState::LoadScene(ScenePtr & ptr_scene)
{
	m_currentScene = std::move(ptr_scene);
}