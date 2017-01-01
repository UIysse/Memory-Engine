#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "DebuggedProcess.h"
#include "ui_ResultInputBoxes.h"
#include "Results.h"

class InputComment : public QDialog
{
	Q_OBJECT
public:
	ResultsWindow * _pResultsWindow;
	InputComment(QDialog * parent, ResultsWindow * pResultsWindow, QTreeWidgetItem * itm, int column);
	~InputComment();
	public slots:
	void confirm();
	void decline();
public:
	QTreeWidgetItem * _itm;
	int _column;
	Ui::CommentBox ui;
};