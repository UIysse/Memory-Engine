#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "MemoryViewer.h"
#include "ui_openProcess.h"
#include "ui_MemoryView.h"
class InputGotoBox : public QDialog
{
	Q_OBJECT
public:
	MemoryViewer * pMemoryViewer;
	void foo();
	InputGotoBox::InputGotoBox(QMainWindow * parent, MemoryViewer * pMemoryViewer);
	~InputGotoBox();
	GotoBox ui;
};