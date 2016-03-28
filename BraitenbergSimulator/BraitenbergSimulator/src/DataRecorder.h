#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <vector>
#include <map>

#include <memory>

struct CSVRow
{
	std::vector<std::string> m_cellData;
	std::string GetRow();
};


struct StreamData
{
	std::ofstream m_stream;
	int m_columns;
	std::string m_filePath;
};


typedef std::unique_ptr<StreamData> StreamDataPtr;

class DataRecorder
{
public:
	DataRecorder();
	DataRecorder(std::string directoryPath);

	void BeginFile(CSVRow headerRow, std::string identifier);
	void Record(CSVRow row, std::string identifier);
	void EndFile(std::string identifier);
	void CloseAll();
private:
	std::ofstream m_outputStream;
	std::string m_directoryPath;
	std::map<std::string, StreamDataPtr> m_streams; //identifier, stream
	int m_columns = 0;
};