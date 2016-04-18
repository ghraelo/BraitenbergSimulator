#include "NoVisualisationState.h"
#include "MenuState.h"


#include "imgui.h"
#include "SimEngine.h"
#include "ResourceManager.h"
#include "MathUtils.h"
#include "Statistics.h"


NoVisualisationState::NoVisualisationState()
{

}

void NoVisualisationState::Init(SimEngine & se)
{
	//set default scene file
	std::vector<std::string> out;
	ResourceManager::GetFilesInDirectory(out, "yaml");
	if (out.size() > 0)
	{
		uim.PopulateList(out);
	}
	else
	{
		printf("error: no scene files found\n");
	}

	prevMouseState = se.GetMouseState();

	printf("number of cores: %s\n", getenv("NUMBER_OF_PROCESSORS"));

	//register events
	StringVectorFunc runFunc = [&](StringVector& str){
		testQueue.Begin(str,m_baseSettings.slider);
		printf("help!\n");
	};

	VoidFunc exitFunc = [&]() {
		exiting = true;
	};

	uim.RegisterRun(runFunc);
	uim.RegisterExit(exitFunc);
}

void NoVisualisationState::Cleanup()
{
	testQueue.End();
}

void NoVisualisationState::Update(SimEngine & se)
{

	m_baseSettings.frameTime = se.GetFrameTime();
	
	testQueue.Update();

	if (testQueue.IsRunning())
	{
		m_baseSettings.testNumber = testQueue.GetTestNo();
		m_baseSettings.simTime = testQueue.GetCurrentThread()->GetElapsedTime();
	}

	if (testQueue.IsFinished() && m_baseSettings.testsDone == false)
	{
		m_baseSettings.testNumber = testQueue.GetTestNo();
		m_baseSettings.endTime = glfwGetTime();
		m_baseSettings.testsDone = true;
	}

	if (exiting)
	{
		SimStatePtr p = std::make_unique<MenuState>();
		ChangeState(se, p);
		return;
	}
}

void NoVisualisationState::Draw(SimEngine & se)
{
	NVGcontext* vg = se.GetContext();

	// draw interface here
	imguiBeginFrame(se.GetMouseState().xPos, se.GetMouseState().yPos, se.GetMouseState().leftMouse, 0, &guiRenderer);
	uim.Draw(m_baseSettings, se.GetWindowState());

	imguiEndFrame();

	guiRenderer.Flush(vg);
}

void NoVisualisationState::HandleEvents(SimEngine & se)
{
}

void NoVisualisationState::OnScroll(double scrollOffset)
{
}