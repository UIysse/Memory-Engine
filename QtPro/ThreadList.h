#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_ThreadList.h"
class ThreadListWindow : public QDialog
{
	//Q_OBJECT
public:
	ThreadListWindow(QMainWindow* parent = 0) : QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
		UpdateThreadList();
	}
	~ThreadListWindow() {}
	//functionalities
	void UpdateThreadList();
private:
	Ui::Ui_ThreadList ui;
};