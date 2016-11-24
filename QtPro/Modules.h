#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_Modules.h"
class ModulesWindow : public QDialog
{
	//Q_OBJECT
public:
	void UpdateModulesList();
	ModulesWindow(QMainWindow* parent = 0) //: QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
		UpdateModulesList();
	}
	~ModulesWindow() {}
private:
	Ui::DialogModules ui;
};