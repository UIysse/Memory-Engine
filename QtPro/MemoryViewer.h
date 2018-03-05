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
	void FreeOneMemoryBlock(MEMBLOCK * mb);
	void FreeMemoryBlocks(MEMBLOCK * mb_list);
	int MemoryViewer::insertDisas(MemoryViewer * aDialog);
	MEMBLOCK * MemoryViewer::QueryMemoryAddrress(int64_t addr);
	MEMBLOCK * MemoryViewer::create_memblockInsertDisas(HANDLE hProc, MEMORY_BASIC_INFORMATION * meminfo, int data_size);
	~MemoryViewer();
	void DisassembleNewContent();
	MemoryView ui;
	public slots:
	void showDlls();
	void ShowGotoDialogBox();
};