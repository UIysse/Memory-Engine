#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "DebuggedProcess.h"
#include "Results.h"
#include "Modules.h"
#include "PointerHolder.h"
#include "MemRead.h"
#include "Search.h"
#include "InputComment.h"
#include "TypeBox.h"
#include "DebuggedProcess.h"
#include "MyMutexes.h"
#include "CheckPerformance.h"
#include <mutex>
#include <thread>
#include <cstdlib>
ResultsWindow::~ResultsWindow()
{
	mResultsVec.lock();
	mSavedVec.lock();
	_pHoldPtr->_bGlobalSearchInstance = false;
	_ThreadStayAlive = 0;
	mSavedVec.unlock();
	mResultsVec.unlock(); //in case global search is performing
}
ResultsWindow::ResultsWindow(QMainWindow* parent, HoldPtr *pHoldPtr /*= 0*/) //: QDialog(parent)
{
	_pHoldPtr = pHoldPtr;
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	this->setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	this->move(0, 0);
	ui.setupUi(this);
	ui._vecResultsAddr.reserve(NUMBER_DISPLAYED_RESULTS);
	ui._vecSavedAddr.reserve(NUMBER_DISPLAYED_RESULTS); // this means user cannot saved more than 1500 addr or function will not remain thread safe
	_ThreadStayAlive = 1;
}
void ResultsWindow::closeEvent(QCloseEvent *event)
{
	//saves scan settings here
	if (pSearchWindow != nullptr)
	{
		pSearchWindow->_Dialog->close();
		pSearchWindow = nullptr;
	}
}