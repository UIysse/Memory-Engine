#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QMessageBox>
#include "ui_Search.h"
#include "Search.h"
#include "ui_MemoryView.h"
#include <Windows.h>
#include <winnt.h>
#include <string>
#include <sstream>
//class InputComment;
class ResultsWindow : public QDialog
{
	Q_OBJECT
public:
	ResultsWindow(QMainWindow* parent = 0);
	
	public slots:
	void AddVariable(QTreeWidgetItem * itm, int column)
	{
		ui.itm = new QTreeWidgetItem(ui.treeWidget_2);
		ui.itm->setText(1, itm->text(0));
		ui.itm->setText(2, itm->text(1));
		ui.itm->setText(3, pSearchWindow->comboBValueType->currentText());
	}
	void AddComment(QTreeWidgetItem * itm, int column);
public:
	~ResultsWindow() {}
public:
	Ui_DialogResults ui;
	Ui_DialogSearch *pSearchWindow;
	friend void print_matches(MEMBLOCK *mb_list, ResultsWindow* pResultWindow);
};
