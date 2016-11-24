#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_Search.h"
class SearchWindow : public QDialog
{
	Q_OBJECT
public :
	SearchWindow(QMainWindow* parent = 0) : QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
		QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(close()));
		QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close()));
	}
	~SearchWindow() {}
private:
	Ui::Ui_Search ui;
};