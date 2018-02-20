#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QFontDatabase>
#include "ui_Logs.h"
#include "MemRead.h"
#include <fstream>
#include "LogsOutput.h" //if we forward declare the class Logsoutput we will need to put constructor of Logs in a .cpp file
#include "DebuggedProcess.h"
#define LOUT LOUTlog()

PRXY LOUTlog();
class Logs : public QDialog
{
	Q_OBJECT
public:
	LogsOutput * pLogsOutput;
	Logs(QMainWindow * parent = 0) : pLogsOutput(new LogsOutput)//: QDialog(parent)
	{
		pLogsOutput->pLogs = this;
		DebuggedProc.pLogsOutput = pLogsOutput;
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
	}
	bool UpdateTextContent(std::string &strArg) {
		ui.str2 += strArg;
		//
		QTextDocument *doc = ui.plainTextEdit->document();
		QFont font("Terminal");
		font.setPointSize(9);
		font.setStretch(145);
		font.setKerning(false);
		ui.plainTextEdit->setFont(font);
		ui.plainTextEdit->setPlainText(ui.str2.c_str());
		ui.plainTextEdit->show();
		QTextCursor c = ui.plainTextEdit->textCursor(); // Copy assignement operator of QTextCursor gets called.
		c.movePosition(QTextCursor::End);
		ui.plainTextEdit->setTextCursor(c);
		return true;
	}
	~Logs(){
		delete pLogsOutput;
		DebuggedProc.pLogsOutput = nullptr;
		pLogsOutput = nullptr;
	}
private:
	Ui::LogsForm ui;
};
