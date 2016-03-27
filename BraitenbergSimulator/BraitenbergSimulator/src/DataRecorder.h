#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
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

	void BeginFile(CSVRow headerRow, std::string identifier);
	void Record(CSVRow row, std::string identifier);
private:
	std::ofstream m_outputStream;
	std::string m_directoryPath;
	std::map<std::string, std::string> m_filePaths; //identifier, filePath
	int m_columns = 0;
};