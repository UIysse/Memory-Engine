#pragma once
#include "Logs.h"
#include "ui_openProcess.h"
#include "ui_MemoryView.h"
#include "ThreadList.h"
#include "MemRead.h"
#include <atomic>
#include <fstream>

class MemoryViewer : public QMainWindow
{
	Q_OBJECT
public:
	MemoryViewer(QWidget *parent = 0);
	~MemoryViewer();
	void UpdateDisassembledContent();
	MemoryView ui;
	public slots:
	void showDlls();
	void ShowGotoDialogBox();
};