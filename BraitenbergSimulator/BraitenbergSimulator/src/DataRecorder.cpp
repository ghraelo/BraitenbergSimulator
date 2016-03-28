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

	StreamDataPtr s = std::make_unique<StreamData>();
	s->m_stream;
	s->m_filePath = "logs/log-" + identifier + "-" + oss.str() + ".csv";
	s->m_columns = headerRow.m_cellData.size();

	m_streams.emplace(identifier, std::move(s));

	//open each stream and write header row
	
	m_streams[identifier]->m_stream.open(m_directoryPath + m_streams[identifier]->m_filePath);
	m_streams[identifier]->m_stream << headerRow.GetRow();

}

void DataRecorder::Record(CSVRow row, std::string identifier)
{
	assert(row.m_cellData.size() == m_streams[identifier]->m_columns);

	m_streams[identifier]->m_stream << row.GetRow();
}

void DataRecorder::EndFile(std::string identifier)
{
	m_streams[identifier]->m_stream.close();
}

void DataRecorder::CloseAll()
{
	for (auto& streamData : m_streams)
	{
		streamData.second->m_stream.close();
	}
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
