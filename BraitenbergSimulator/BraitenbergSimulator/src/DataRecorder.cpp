#include "DataRecorder.h"

#include <ctime>
#include <cassert>
#include <sstream>

DataRecorder::DataRecorder()
	:m_directoryPath("")
{
}

DataRecorder::DataRecorder(std::string directoryPath)
	:m_directoryPath(directoryPath)
{
}

void DataRecorder::BeginFile(CSVRow headerRow, std::string identifier)
{
	time_t now = time(0);
	tm* dt = localtime(&now);

	std::ostringstream oss;

	oss << 1900 + dt->tm_year;
	oss << 1 + dt->tm_mon;
	oss << dt->tm_mday;
	oss << "_";
	oss << dt->tm_hour;
	oss << dt->tm_min;
	oss << dt->tm_sec;

	std::string currentFile = "logs/log-" + identifier + "-" + oss.str() + ".csv";

	m_filePaths.insert(std::pair<std::string,std::string>(identifier, currentFile));

	m_outputStream.open(m_directoryPath + currentFile);
	if (m_outputStream.fail())
	{
		int a = 5;
	}
	m_outputStream << headerRow.GetRow();
	m_outputStream.close();

	m_columns = headerRow.m_cellData.size();
}

void DataRecorder::Record(CSVRow row, std::string identifier)
{
	assert(row.m_cellData.size() == m_columns);

	m_outputStream.open(m_directoryPath + m_filePaths[identifier], std::ios::app);
	m_outputStream << row.GetRow();
	m_outputStream.close();
}

std::string CSVRow::GetRow()
{
	std::string temp;
	for (int i = 0; i < m_cellData.size();i++)
	{
		temp += m_cellData[i] + ",";
	}

	temp += "\n";

	return temp;
}
