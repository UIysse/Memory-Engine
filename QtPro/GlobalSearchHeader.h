#pragma once
#include <thread>
#include "Search.h"
#include "Results.h"
class GlobalSearchInstance
{
public:
	//objects
	HoldPtr *_HoldPtr;
	ResultsWindow *_pResultsWindow;
	SearchWindow *_pSearchWindow;
	std::thread* _th;
	std::thread* _th2;
	std::atomic<bool> _ThreadStayAlive; // put it in ResultsWindow and let threads here
	//functions
	GlobalSearchInstance() = delete;
	GlobalSearchInstance(QMainWindow *parent, HoldPtr *pHoldPtr);
	void ActivateWindows(); //Will bring Result and Search dialogs to front.
	~GlobalSearchInstance();
};