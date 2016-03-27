#include "NoVisualisationState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"
#include "MathUtils.h"
#include "Statistics.h"

#include <thread>

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

	ScenePtr s = ResourceManager::LoadScene(m_baseSettings.activeSceneFilename);

	simManager = std::make_unique<SimManager>(s);


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
		headerRow.m_cellData.push_back("events");
		headerRow.m_cellData.push_back("t");
		headerRow.m_cellData.push_back("x");
		headerRow.m_cellData.push_back("y");
		headerRow.m_cellData.push_back("angle");
		headerRow.m_cellData.push_back("distance travelled");
		dr.BeginFile(headerRow, obj->GetName());
	}

	m_baseSettings.startTime = glfwGetTime();

	prevMouseState = se.GetMouseState();

	std::thread q(&NoVisualisationState::ThreadTest, this, "help");
	q.detach();

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

	for (int i = 0; i < 60; i++)
	{
		simManager->Step(1.0f / 60);
		simTime += 1.0f / 60;

		for (auto &obj : simManager->GetCurrentScene()->m_vehicles)
		{
			CSVRow dataRow;
			std::stringstream conv;

			//events
			if (simManager->GetEventFlags() == EF_BoundaryCollision)
			{
				conv << "B";
			}
			dataRow.m_cellData.push_back(conv.str());
			conv.str(std::string());
			conv.clear();

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

			dr.Record(dataRow, obj->GetName());
		}

		sm.Update();
	}
	printf("simTime: %f s\n", simTime);	
}

void NoVisualisationState::Draw(SimEngine & se)
{
	printf("frame time: %0.2f s\n", se.GetFrameTime());
	printf("update time: %0.2f s\n", se.GetUpdateTime());
	printf("draw time: %0.2f s\n", se.GetDrawTime());

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

void NoVisualisationState::ThreadTest(std::string msg)
{
	std::cout << "task1 says: " << msg << std::endl;
}

void NoVisualisationState::LoadScene(ScenePtr & ptr_scene)
{
	m_currentScene = std::move(ptr_scene);
}
