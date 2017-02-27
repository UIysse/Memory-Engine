#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "DebuggedProcess.h"
#include "ui_ResultInputBoxes.h"
#include "Results.h"

class InputTypeBox : public QDialog
{
	Q_OBJECT
public:
	ResultsWindow * _pResultsWindow;
	InputTypeBox(QDialog * parent, ResultsWindow * pResultsWindow, QTreeWidgetItem * itm, int column);
	~InputTypeBox();
	public slots:
	void confirm();
	void decline();
public:
	QTreeWidgetItem * _itm;
	int _column;
	Ui::TypeBox ui;
};