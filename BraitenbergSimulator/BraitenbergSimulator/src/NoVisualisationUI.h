#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <map>

#include "WMState.h"
#include "NoVisUISettings.h"

typedef std::vector<std::string> StringVector;
typedef std::unique_ptr<int> IntPtr;
typedef std::function<void(std::string)> StringFunc;
typedef std::function<void(std::vector<std::string>)> StringVectorFunc;
typedef std::function<void()> VoidFunc;

class NoVisualisationUI
{
public:
	NoVisualisationUI();
	void RegisterClear(VoidFunc clear);
	void RegisterRun(StringVectorFunc run);
	void RegisterExit(VoidFunc exit);
	void Draw(NoVisUISettings settings, WindowState& ws);
	void PopulateList(StringVector& fileNames);
private:
	void OnClear();
	void OnRun(StringVector tests);
	void OnExit();

	StringFunc m_add;
	VoidFunc m_clear;
	VoidFunc m_exit;
	StringVectorFunc m_run;

	StringVector m_availableFileNames;
	StringVector m_inUseNames;

	bool m_testRunning = false;
};