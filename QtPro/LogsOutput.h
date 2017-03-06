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
	friend LogsOutput& operator<<(LogsOutput &logsClass, uint64_t number);
};
class PRXY : public LogsOutput
{
public :
		PRXY(){}
		~PRXY() {}
	friend PRXY& operator<<(PRXY &prxyClass, std::string &strArg);
	friend PRXY& operator<<(PRXY &prxyClass, const char *strArg);
	friend PRXY& operator<<(PRXY &prxyClass, ostream_manipulator2 pf);
	friend PRXY& operator<<(PRXY &prxyClass, uint64_t number);
	friend PRXY& operator >> (PRXY &logsClass, uint64_t number);
};