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
	void UpdateResultsValue();
	void UpdateSavedValue();
	void closeEvent(QCloseEvent *event); //Maintain this name (no upper C letter) because it is overloading Qt's function
	ResultsWindow() = delete;
	ResultsWindow(QMainWindow* parent, HoldPtr *pHoldPtr);
	~ResultsWindow();
	public slots:
	void SetValues(QTreeWidgetItem * itm, unsigned long long nValue, int column, bool bValidMemory)//for some reason connect wont work if we use uint64_t here -> using ulong long
	{
		if (bValidMemory == 0)
		{
			itm->setText(column, "???");
			return;
		}
		QString text = itm->text(column);
		if (text.toULongLong(0, 16) == nValue)
			return; //changed values will now appear as red, nothing is changed otherwise //doesn't work because nValue is always 0
		itm->setText(column, ReturnStrFromHexaInt(nValue).c_str());
		if (column == 1)
		itm->setTextColor(column, Qt::red);
	}
signals:
	void UpdateResultsContent(QTreeWidgetItem *, unsigned long long, int, bool);
public:
	HoldPtr *_pHoldPtr;
	std::atomic<bool> _ThreadStayAlive;
	Ui_DialogResults ui;
	Ui_DialogSearch *pSearchWindow;
	friend void print_matches(MEMBLOCK *mb_list, ResultsWindow* pResultWindow);
};
