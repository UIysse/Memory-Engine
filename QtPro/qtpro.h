#ifndef QTPRO_H
#define QTPRO_H
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "AboutCpp.h"
#include "ui_qtpro.h"
#include "Logs.h"
#include "PointerHolder.h"
//#include "ui_openProcess.h"
//#include "ui_MemoryView.h"
#include "ThreadList.h"
#include "MemRead.h"
#include <atomic>
#include <fstream>
//#include "ui_ShowDLLs.h
class ResultsWindow; //forward declaring those classes for sole pointer use works whereas including their headers results in compiler error
class SearchWindow;
class MemoryViewer; // allows declaration prototype for inserDisas
int insertDisas(MemoryViewer * aDialog);
HANDLE  ReturnProcessHandle(QString Qstr);
class QtPro : public QMainWindow
{
	Q_OBJECT
public:
	Logs *pLogsWindow;
	SearchWindow *_pSearchWindow;
	ResultsWindow *_pResultsWindow;
	HoldPtr _HoldPtr;
	QtPro(QWidget *parent = 0);
	void closeDialog();
	~QtPro();
	static void OpenHandle();
	private slots:
	void ShowAboutDialog();
	void ShowPickProcess();
	void ShowMemoryView(); 
	void ShowLogs();
	void ShowSearch();
	void ShowModules();
	void ShowThreadList();
	void ShowMemoryMap();
	void UpdateProcessName();
	void closeEvent(QCloseEvent *event)
	{
			QApplication::closeAllWindows();
	}
public:
	Ui::QtProClass ui;
};
#endif // QTPRO_H
