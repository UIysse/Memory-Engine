#include "OpenProcess.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "DebuggedProcess.h"
#include <string>
#include <iostream>
void OpenProcessClass::SelectProcess()
{
	QString strChosenProcess = this->ui.listView->currentItem()->text(2);
	DebuggedProc.hwnd = ReturnProcessHandle(strChosenProcess);
	pQtPro->ui.lblProcessName->setText(strChosenProcess);
	this->close();
}
void OpenProcessClass::CancelProcessSelection()
{
	this->close();
}
OpenProcessClass::OpenProcessClass(QWidget *parent, QtPro* MainWind) : QDialog(parent)
{
	pQtPro = MainWind;
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	ui.setupUi(this);
	QObject::connect(ui.pushButton_2, &QPushButton::clicked, this, &OpenProcessClass::SelectProcess);
	QObject::connect(ui.pushButton, &QPushButton::clicked, this, &OpenProcessClass::CancelProcessSelection);
}