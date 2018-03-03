#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QFontDatabase>
#include "ui_Logs.h"
#include "MemRead.h"
#include "PointerHolder.h"
#include "qtpro.h"
#include <fstream>
#include "LogsOutput.h" //if we forward declare the class Logsoutput we will need to put constructor of Logs in a .cpp file
#include "DebuggedProcess.h"
#define LOUT LOUTlog()
#define LOUT_ERROR LOUTlog(LogTypes::ErrorLog)
class HoldPtr;
PRXY LOUTlog(LogTypes log = LogTypes::NormalLog); //funciton declaration
class Logs : public QDialog
{
	Q_OBJECT
public:
	LogsOutput * pLogsOutput;
	HoldPtr *_pHoldPtr;
	Logs(QMainWindow * parent = 0) : pLogsOutput(new LogsOutput)//: QDialog(parent)
	{
		pLogsOutput->pLogs = this;
		DebuggedProc.pLogsOutput = pLogsOutput;
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
	}
	Logs(HoldPtr * pHoldPtr, QMainWindow * parent = 0) : pLogsOutput(new LogsOutput)//: QDialog(parent)
	{
		_pHoldPtr = pHoldPtr;
		pLogsOutput->pLogs = this;
		DebuggedProc.pLogsOutput = pLogsOutput;
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
	}
	bool UpdateTextContent(std::string &strArg, LogTypes logType = LogTypes::NormalLog);
	~Logs(){
		_pHoldPtr->_bLogsWindow = false;
		delete pLogsOutput;
		DebuggedProc.pLogsOutput = nullptr;
		pLogsOutput = nullptr;
	}
private:
	Ui::LogsForm ui;
};
