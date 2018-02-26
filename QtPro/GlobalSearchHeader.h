#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <thread>
#include "Search.h"
#include "Results.h"
#include <qobject.h>
class GlobalSearchInstance : public QObject
{
	Q_OBJECT
public:
	//objects
	QMainWindow *_pQMainWindow;
	HoldPtr *_HoldPtr;
	ResultsWindow *_pResultsWindow;
	SearchWindow *_pSearchWindow;
	std::thread* _th;
	std::thread* _th2;
	//functions
	GlobalSearchInstance() = delete;
	GlobalSearchInstance(QMainWindow *parent, HoldPtr *pHoldPtr);
	void ActivateWindows(); //Will bring Result and Search dialogs to front.
	void GlobalSearchInstance::UpdateResultsValue();
	//void GlobalSearchInstance::SetValues(QTreeWidgetItem * itm, unsigned long long nValue, int column, bool bValidMemory);
	void GlobalSearchInstance::UpdateSavedValue();
	void GlobalSearchInstance::ConnectSlots(); //call this ft after ui are setup.
	~GlobalSearchInstance();
	//slots
	public slots:
	void AddVariable(QTreeWidgetItem * itm, int column);
	void AddComment(QTreeWidgetItem * itm, int column);
	void GlobalSearchInstance::SetValues(QTreeWidgetItem * itm, unsigned long long nValue, int column, bool bValidMemory);
	signals:
	void signalItemChange(QTreeWidgetItem * itm, unsigned long long nValue, int column, bool bValidMemory);
};