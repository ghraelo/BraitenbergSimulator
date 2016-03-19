#include "MainState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"
#include "MathUtils.h"
#include "Statistics.h"

MainState::MainState()
	:sm(30)
{
}

void MainState::Init(SimEngine & se)
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

	cam = std::make_unique<Camera>(se.GetWindowState().width,se.GetWindowState().height);

	m_sceneRenderer.SetCamera(cam.get());

	for (auto &obj : m_currentScene->m_vehicles)
	{
		std::function<float()> angleCallback = [&]() {
			return atan2(obj->m_body->GetWorldVector(b2Vec2(0, 1)).y, obj->m_body->GetWorldVector(b2Vec2(0, 1)).x);
		};

		std::function<float()> distCallback = [&]() {
			return b2Distance(obj->m_body->GetPosition(), m_currentScene->m_lights[0].GetPosition());
		};


		//add statistics watchers
		StatisticPtr s = std::make_unique<FixedWatcher>(obj->GetName() + " - angle", 10, 170,
			angleCallback, -M_PI, M_PI);

		StatisticPtr s2 = std::make_unique<Watcher>(obj->GetName() + " - distance", 10, 170,
			distCallback);

		StatisticPtr p1 = std::make_unique<AutocorrelationProcessor>(obj->GetName() + " - acor(distance)", dynamic_cast<Watcher*>(s2.get()));

		StatisticPtr p2 = std::make_unique<AutocorrelationProcessor>(obj->GetName() + " - acor(angle)", dynamic_cast<FixedWatcher*>(s.get()));

		StatisticPtr p3 = std::make_unique<PeriodicityDetectionProcessor>(obj->GetName() + " - dct(angle)", dynamic_cast<Watcher*>(s.get()));

		sm.AddStat(std::move(s));
		sm.AddStat(std::move(s2));
		sm.AddStat(std::move(p1));
		sm.AddStat(std::move(p2));
		//sm.AddStat(std::move(p3));
	}

	prevMouseState = se.GetMouseState();
}

void MainState::Cleanup()
{
}

void MainState::Update(SimEngine & se)
{
	m_baseSettings.frameTime = se.GetFrameTime();
	if (m_baseSettings.shouldExit)
	{
		SimStatePtr p = std::make_unique<MenuState>();
		ChangeState(se, p);
		return;
	}
	world->Step(1.0f/60, 10, 10);
;	for (auto &obj : m_currentScene->m_vehicles)
	{
		Rectangle bounds(cam->ConvertScreenToWorld(cam->GetRect().m_topRight), cam->ConvertScreenToWorld(cam->GetRect().m_bottomLeft));
		obj->Update(m_currentScene->m_lights,bounds);

		/* NEED SCREEN TO WORLD FUNCTION
		//wrap sides
		b2Vec2 pos = obj->GetPosition();

		Rectangle bounds = cam->GetRect();
		printf("pos: (%f,%f)\n",pos.x,pos.y);
		
		if (pos.x < bounds.GetSidePos(RS_LEFT))
		{
			obj->SetPosition(b2Vec2(bounds.GetSidePos(RS_RIGHT), pos.y));
			printf("less than LEFT: %f", bounds.GetSidePos(RS_LEFT));

		}
		else if (pos.x > bounds.GetSidePos(RS_RIGHT))
		{
			obj->SetPosition(b2Vec2(bounds.GetSidePos(RS_LEFT), pos.y));
			printf("greater than RIGHT: %f", bounds.GetSidePos(RS_RIGHT));

		}
		else if (pos.y > bounds.GetSidePos(RS_BOTTOM))
		{
			obj->SetPosition(b2Vec2(pos.x, bounds.GetSidePos(RS_TOP)));
			printf("greater than BOTTOM: %f", bounds.GetSidePos(RS_BOTTOM));

		}
		else if (pos.y < bounds.GetSidePos(RS_TOP))
		{
			obj->SetPosition(b2Vec2(pos.x, bounds.GetSidePos(RS_BOTTOM)));
			printf("less than TOP: %f", bounds.GetSidePos(RS_TOP));

		}*/
	}
	sm.Update();
}

void MainState::Draw(SimEngine & se)
{
	NVGcontext* vg = se.GetContext();

	//nvgTranslate(vg, se.GetWindowState().width/2, se.GetWindowState().height);
	nvgScale(vg, 1.0f, -1.0f);
	//render vehicles

	for (auto &obj : m_currentScene->m_lights)
	{
		obj.Render(vg, m_sceneRenderer);
	}

	for (auto &obj : m_currentScene->m_vehicles)
	{
		obj->Render(vg, m_sceneRenderer);
	}

	nvgScale(vg, 1.0f, -1.0f);

	// draw interface here
	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
	uim.DrawBaseUI(m_baseSettings, se.GetWindowState());

	if (m_baseSettings.showStatsPane)
		uim.DrawStatsPane(sm, se.GetWindowState());

	imguiEndFrame();

	guiRenderer.Flush(vg);
	Rectangle r = cam->GetRect();
}

void MainState::HandleEvents(SimEngine & se)
{
	MouseState currState = se.GetMouseState();

	//mouse dragging
	if (currState.rightMouse == GLFW_PRESS && m_dragging == false)
		m_dragging = true;

	if (currState.rightMouse == GLFW_RELEASE && m_dragging == true)
		m_dragging = false;

	if (m_dragging)
	{
		b2Vec2 currOrigin = cam->GetOrigin();
		b2Vec2 increment(0, 0);

		float diffX = currState.xPos - prevMouseState.xPos;
		float diffY = currState.yPos - prevMouseState.yPos;

		if (diffX < 0)
		{
			increment.x = diffX;
		}
		else if(diffX > 0)
		{
			increment.x = diffX;
		}

		if (diffY < 0)
		{
			increment.y = diffY;
		}
		else if (diffY > 0)
		{
			increment.y = diffY;
		}

		cam->SetOrigin(currOrigin + increment);
	}

	prevMouseState = se.GetMouseState();
}

void MainState::OnScroll(double scrollOffset)
{
	float zoom = cam->GetZoom();
	if (scrollOffset < 0)
	{
		cam->SetZoom(zoom / fabs(scrollOffset * 0.5));
	}
	else
	{
		cam->SetZoom(zoom * fabs(scrollOffset * 0.5));
	}
}

void MainState::LoadScene(ScenePtr & ptr_scene)
{
	m_currentScene = std::move(ptr_scene);
}
