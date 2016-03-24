#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
struct CSVRow
{
	std::vector<std::string> m_cellData;
	std::string GetRow();
};

class DataRecorder
{
public:
	DataRecorder();
	DataRecorder(std::string directoryPath);

	void BeginFile(CSVRow headerRow);
	void Record(CSVRow row);
private:
	std::ofstream m_outputStream;
	std::string m_directoryPath;
	std::string m_currentFile;
	int m_columns = 0;
};