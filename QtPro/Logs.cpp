//#include "MemRead.h"
#include <fstream>
#include "Logs.h"
#include "LogsOutput.h"
#include "DebuggedProcess.h"
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
bool Logs::UpdateTextContent(std::string &strArg, LogTypes logType /*= LogTypes::NormalLog*/) {
	ui.str2 += strArg;
	std::string addedLine = "";
	std::string errorHtml = "<font color=\"Red\">";
	std::string endHtml = "</font><br>";
	//
	QTextDocument *doc = ui.plainTextEdit->document(); //Returns a pointer to the underlying document.
	QFont font("Terminal");//Should move these code lines to setup function
	font.setPointSize(9);
	font.setStretch(145);
	font.setKerning(false);
	ui.plainTextEdit->setFont(font);
	//ui.plainTextEdit->setPlainText(ui.str2.c_str());
	ui.plainTextEdit->moveCursor(QTextCursor::End); //equivalent to next code lines with QTextCursor c
	switch (logType)
	{
	case LogTypes::ErrorLog:
		//Format html string
		addedLine += errorHtml += strArg += endHtml;
		strArg = addedLine;
		ui.plainTextEdit->appendHtml(strArg.c_str());
		break;
	default:
		ui.plainTextEdit->insertPlainText(strArg.c_str());
	}
	ui.plainTextEdit->show();
	QTextCursor c = ui.plainTextEdit->textCursor(); // Copy assignement operator of QTextCursor gets called.
	c.movePosition(QTextCursor::End);
	ui.plainTextEdit->setTextCursor(c);
	return true;
}

PRXY& operator<<(PRXY &logsClass, std::string &strArg) {
	if (DebuggedProc.pLogsOutput)
	{
		fout << strArg;
		DebuggedProc.pLogsOutput->pLogs->UpdateTextContent(strArg, logsClass._eLogType);
	}
	else
	{
		fout << strArg;
	}
	return logsClass;
}
PRXY& operator<<(PRXY &logsClass, ostream_manipulator pf)
{
	if (DebuggedProc.pLogsOutput)
	{
		fout << endl;
		std::string str = "\n";
		DebuggedProc.pLogsOutput->pLogs->UpdateTextContent(str);
	}
	else
	{
		fout << endl;
	}
	return logsClass;
}
PRXY& operator<<(PRXY &logsClass, const char *strArg)
{
	if (DebuggedProc.pLogsOutput)
	{
		fout << strArg;
		std::string str(strArg);
		DebuggedProc.pLogsOutput->pLogs->UpdateTextContent(str, logsClass._eLogType);
	}
	else
	{
		fout << strArg;
	}
	return logsClass;
}

PRXY& operator<<(PRXY &logsClass, uint64_t number) {
	if (DebuggedProc.pLogsOutput)
	{
		fout << number;
		stringstream ss;
		ss << std::dec << number;
		std::string str = ss.str();
		DebuggedProc.pLogsOutput->pLogs->UpdateTextContent(str);
	}
	else
	{
		fout << number;
	}
	return logsClass;
}
PRXY& operator>>(PRXY &logsClass, uint64_t number) {
	if (DebuggedProc.pLogsOutput)
	{
		fout << number;
		stringstream ss;
		ss << std::hex << number;
		std::string str = ss.str();
		DebuggedProc.pLogsOutput->pLogs->UpdateTextContent(str);
	}
	else
	{
		fout << number;
	}
	return logsClass;
}
PRXY LOUTlog(LogTypes log /*= LogTypes::NormalLog*/) {
	PRXY a(log);
	return a;
}