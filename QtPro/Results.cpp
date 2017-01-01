#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "DebuggedProcess.h"
#include "Results.h"
#include "Modules.h"
#include "MemRead.h"
#include "Search.h"
#include "InputComment.h"


ResultsWindow::ResultsWindow(QMainWindow* parent /*= 0*/) //: QDialog(parent)
{
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	this->setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
	this->move(0, 0);
	ui.setupUi(this);
	//must be placed after setupUi (probably the prepocessor needs treewidget pointer to actually know where it will point
	QObject::connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(AddVariable(QTreeWidgetItem *, int)));
	QObject::connect(ui.treeWidget_2, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(AddComment(QTreeWidgetItem *, int)));
}

void ResultsWindow::AddComment(QTreeWidgetItem * itm, int column)
{
	InputComment * pInputComment;
	switch (column)
	{
	case 0:
		pInputComment = new InputComment(this, this, itm, column);
		pInputComment->setWindowFlags(pInputComment->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		pInputComment->setAttribute(Qt::WA_DeleteOnClose);
		pInputComment->show();
		pInputComment->ui.textEdit->setFocus();
		break;
	case 2:
		pInputComment = new InputComment(this, this, itm, column);
		pInputComment->setWindowFlags(pInputComment->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		pInputComment->setAttribute(Qt::WA_DeleteOnClose);
		pInputComment->show();
		pInputComment->ui.textEdit->setFocus();
		break;
	}
}