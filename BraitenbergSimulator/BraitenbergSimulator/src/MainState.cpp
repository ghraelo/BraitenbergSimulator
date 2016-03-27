#include "MainState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"
#include "MathUtils.h"
#include "Statistics.h"

#include <sstream>

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

	ScenePtr s = ResourceManager::LoadScene(m_baseSettings.activeSceneFilename);

	simManager = std::make_unique<SimManager>(s);

	cam = std::make_unique<Camera>(se.GetWindowState().width,se.GetWindowState().height);

	m_sceneRenderer.SetCamera(cam.get());

	for (auto &obj : simManager->GetCurrentScene()->m_vehicles)
	{
		std::function<float()> angleCallback = [&]() {
			return atan2(obj->m_body->GetWorldVector(b2Vec2(0, 1)).y, obj->m_body->GetWorldVector(b2Vec2(0, 1)).x);
		};

		std::function<float()> distCallback = [&]() {
			return b2Distance(obj->m_body->GetPosition(), simManager->GetCurrentScene()->m_lights[0].GetPosition());
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

		CSVRow headerRow;
		headerRow.m_cellData.push_back("t");
		headerRow.m_cellData.push_back("x");
		headerRow.m_cellData.push_back("y");
		headerRow.m_cellData.push_back("angle");
		headerRow.m_cellData.push_back("distance travelled");
		headerRow.m_cellData.push_back("events");
		dr.BeginFile(headerRow,obj->GetName());
	}

	m_baseSettings.startTime = glfwGetTime();

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

	//60 hz timestep
	simManager->Step(1.0f / 60);

	for (auto &obj : simManager->GetCurrentScene()->m_vehicles)
	{
		CSVRow dataRow;
		std::stringstream conv;
		
		//time
		conv << glfwGetTime();
		dataRow.m_cellData.push_back(conv.str());
		conv.str(std::string());
		conv.clear();

		//x
		conv << obj->GetPosition().x;
		dataRow.m_cellData.push_back(conv.str());
		conv.str(std::string());
		conv.clear();

		//y
		conv << obj->GetPosition().y;
		dataRow.m_cellData.push_back(conv.str());
		conv.str(std::string());
		conv.clear();

		//angle
		dataRow.m_cellData.push_back("");
		//distance travelled
		dataRow.m_cellData.push_back("");
		//events
		dataRow.m_cellData.push_back("");

		dr.Record(dataRow,obj->GetName());
	}

	sm.Update();
}

void MainState::Draw(SimEngine & se)
{
	NVGcontext* vg = se.GetContext();

	//flip y axis
	nvgScale(vg, 1.0f, -1.0f);

	//render simulation objects
	simManager->Render(vg, m_sceneRenderer);

	//unflip y-axis
	nvgScale(vg, 1.0f, -1.0f);

	// draw interface here
	imguiBeginFrame((int)se.GetMouseState().xPos, (int)se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
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
