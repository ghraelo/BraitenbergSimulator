#include "NoVisualisationUI.h"

#include "imgui.h"

#include <sstream>
#include <iomanip>
#include <ios>

NoVisualisationUI::NoVisualisationUI()
{
}

void NoVisualisationUI::RegisterClear(VoidFunc clear)
{
	m_clear = clear;
}

void NoVisualisationUI::RegisterRun(StringVectorFunc run)
{
	m_run = run;
}

void NoVisualisationUI::RegisterExit(VoidFunc exit)
{
	m_exit = exit;
}

void NoVisualisationUI::Draw(NoVisUISettings& settings, WindowState & ws)
{
	imguiBeginScrollArea("Simulator v0.2", 0, 0, ws.width*0.2, ws.height, &settings.scroll);

	if (imguiButton("Exit", true))
	{
		OnExit();
	}

	imguiSlider("Iterations:", &settings.slider, 0, 960000, 1000, true);

	imguiSeparatorLine();
	imguiLabel("Available tests:");
	if (imguiButton("Add all", true))
	{
		for (int i = 0; i < m_availableFileNames.size(); i++)
		{
			m_inUseNames.push_back(m_availableFileNames[i]);
		}
	}
	for (int i = 0; i < m_availableFileNames.size(); i++)
	{
		std::string fileName = m_availableFileNames[i];
		fileName = fileName.substr(fileName.find_last_of('/') + 1, std::string::npos);
		if (imguiButton(fileName.c_str(), true))
		{
			m_inUseNames.push_back(m_availableFileNames[i]);

		}
	}

	imguiEndScrollArea();

	imguiBeginScrollArea("Test Queue", ws.width - 200, 0, 200, ws.height, &settings.scroll2);
	for (int i = 0; i < m_inUseNames.size(); i++)
	{
		std::string fileName = m_inUseNames[i];
		fileName = fileName.substr(fileName.find_last_of('/') + 1, std::string::npos);

		imguiLabel(fileName.c_str(),(i==settings.testNumber));
		if (i < settings.testNumber)
		{
			imguiIndent();
			imguiLabel("- done");
			imguiUnindent();
		}
		else if (i == settings.testNumber)
		{
			imguiIndent();
			imguiLabel("- running");
			std::ostringstream ss;
			ss << "- elapsed: " << std::fixed << std::setprecision(2) << settings.simTime << " s";
			imguiLabel(ss.str().c_str());
			imguiUnindent();
		}
	}
	if (settings.testsDone)
	{
		imguiSeparatorLine();
		std::ostringstream ss;
		imguiLabel("Tests complete.");
		ss << "Total real time elapsed:" << std::fixed << std::setprecision(2) << settings.endTime - settings.startTime << " s";
		imguiLabel(ss.str().c_str());

	}

	imguiSeparatorLine();

	if (imguiButton("Run", true))
	{
		if (m_inUseNames.size() > 0)
		{
			OnRun(m_inUseNames);
			settings.startTime = glfwGetTime();
		}
	}

	imguiEndScrollArea();
}

void NoVisualisationUI::PopulateList(StringVector & fileNames)
{
	m_availableFileNames.insert(m_availableFileNames.end(), fileNames.begin(), fileNames.end());
}

void NoVisualisationUI::OnClear()
{
	m_clear();
}

void NoVisualisationUI::OnRun(StringVector tests)
{
	m_run(tests);
}

void NoVisualisationUI::OnExit()
{
	m_exit();
}
