#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_MemoryMap.h"
std::string ReturnStrFromHexaInt(int64_t arg);
std::string ReturnStrFromDecInt(int64_t arg);
class MemoryMapWindow : public QDialog
{
public:
	MemoryMapWindow(QMainWindow* parent = 0)  : QDialog(parent)
	{
		ui.setupUi(this);
		UpdateMemoryMap();
	}
	void UpdateMemoryMap();
	~MemoryMapWindow() {}
public:
	Ui::DialogMemoryMap ui;
};