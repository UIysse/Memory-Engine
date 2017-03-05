//#include "MemRead.h"
#include <fstream>
#include "Logs.h"
#include "LogsOutput.h"
#include <iostream>
#include <sstream>
using namespace std;
extern 	std::fstream fout;
LogsOutput& operator<<(LogsOutput &logsClass, std::string &strArg) { 
	fout << strArg;
	logsClass.pLogs->UpdateTextContent(strArg);
	return logsClass; 
}
typedef std::ostream& (*ostream_manipulator)(std::ostream&);
LogsOutput& operator<<(LogsOutput &logsClass, ostream_manipulator pf)
{
	fout << endl;
	std::string str = "\n";
	logsClass.pLogs->UpdateTextContent(str);
	return logsClass;
}
LogsOutput& operator<<(LogsOutput &logsClass, const char *strArg) 
	{
		fout << strArg;
		std::string str(strArg);
		logsClass.pLogs->UpdateTextContent(str);
		return logsClass;
	}

LogsOutput& operator<<(LogsOutput &logsClass, uint64_t number) {
	fout << number;
	stringstream ss;
	ss << std::dec << number;
	std::string str = ss.str();
	logsClass.pLogs->UpdateTextContent(str);
	return logsClass;
}
/*
LogsOutput& operator<<(LogsOutput &logsClass, size_t number) {
	fout << number;
	stringstream ss;
	ss << number;
	std::string str = ss.str();
	logsClass.pLogs->UpdateTextContent(str);
	return logsClass;
}*/