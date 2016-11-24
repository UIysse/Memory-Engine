#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_Logs.h"
class Logs : public QDialog
{
	Q_OBJECT
public:
	Logs(QMainWindow * parent = 0) //: QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
	}
	~Logs(){}

private:
	Ui::LogsForm ui;
};