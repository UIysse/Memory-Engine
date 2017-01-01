#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include "ui_qtpro.h"
#include "ui_openProcess.h"
#include "ui_MemoryView.h"
#include "ThreadList.h"
#include "MemRead.h"
#include <atomic>
#include <fstream>


class OpenProcessClass : public QDialog
{
public:
	QtPro * pQtPro;
	OpenProcessClass(QWidget *parent, QtPro *MainWind);
	~OpenProcessClass() {
		pQtPro->ui.toolButton->setEnabled(true);
	}
	void SelectProcess();
	void CancelProcessSelection();

public:
	Ui2::OpenProcess ui;
};