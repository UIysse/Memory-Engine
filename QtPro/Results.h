#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QMessageBox>
#include "ui_Search.h"
#include "Search.h"
#include "ui_MemoryView.h"
#include "PointerHolder.h"
#include "MyMutexes.h"
#include <Windows.h>
#include <winnt.h>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <atomic>
//class InputComment;
class ResultsWindow : public QDialog
{
	Q_OBJECT
public:
	void closeEvent(QCloseEvent *event); //Maintain this name (no upper C letter) because it is overloading Qt's function
	ResultsWindow() = delete;
	ResultsWindow(QMainWindow* parent, HoldPtr *pHoldPtr);
	~ResultsWindow();
public:
	HoldPtr *_pHoldPtr;
	std::atomic<bool> _ThreadStayAlive;
	Ui_DialogResults ui;
	Ui_DialogSearch *pSearchWindow;
	friend void print_matches(MEMBLOCK *mb_list, ResultsWindow* pResultWindow);
};
