#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_aboutce.h"
class About : public QDialog
{
	Q_OBJECT
public:
	QWidget * QWptr;
	About(QWidget *parent = 0);
	~About();
private:
	Ui::AboutCEClass ui;
};