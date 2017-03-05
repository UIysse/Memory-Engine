#pragma once
class Logs;
typedef std::ostream& (*ostream_manipulator2)(std::ostream&);
class LogsOutput
{
public:
	LogsOutput() {}
	~LogsOutput() {}
	Logs * pLogs;
	friend LogsOutput& operator<<(LogsOutput &logsClass, std::string &strArg);
	friend LogsOutput& operator<<(LogsOutput &logsClass, const char *strArg);
	friend LogsOutput& operator<<(LogsOutput &logsClass, ostream_manipulator2 pf);
	//friend LogsOutput& operator<<(LogsOutput &logsClass, size_t number);
	friend LogsOutput& operator<<(LogsOutput &logsClass, uint64_t number);
	/*
	template <typename T>
	friend LogsOutput& operator<< (LogsOutput& logsClass, T thing);*/
};