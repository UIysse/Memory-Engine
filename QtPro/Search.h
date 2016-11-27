#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QMessageBox>
#include "ui_Search.h"
#include "MemoryMap.h"
#include "MemRead.h"
#include "DebuggedProcess.h"
#include <string>
#include <sstream>
class ResultsWindow;
typedef enum
{
	BYTE_ = 1,
	TWOBYTES_ = 2,
	INT32_ = 3,
	INT64_ = 4,
	FLOAT_ = 5,
	DOUBLE_ = 6,
	STRING_ = 7,
	ArrayBytes_ = 8,
} DATA_SIZE;
typedef enum
{
	COND_EQUALS,
	COND_INCREASED,
	COND_DECREASED,
	COND_UNCONDITIONAL,//unkowninitiavalue
} SEARCH_CONDITION;
typedef enum
{
	NEXT_SCAN,
	NEW_SCAN,
} SCAN_CONDITION;
void print_matches(MEMBLOCK *mb_list, ResultsWindow* pResultWindow);
int ui_run_scan(MEMBLOCK *scan, uint32_t data_size, int64_t start_val, SEARCH_CONDITION start_cond, SCAN_CONDITION scan_condition);
class ResultsWindow : public QDialog
{
	Q_OBJECT
public:
	ResultsWindow(QMainWindow* parent = 0) //: QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		this->move(0, 0);
		ui.setupUi(this);
	}
	~ResultsWindow() {}
public:
	Ui::Ui_Results ui;
	friend void print_matches(MEMBLOCK *mb_list, ResultsWindow* pResultWindow);
};
class SearchWindow : public QDialog
{
	Q_OBJECT
public :
	void someSlot() {
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Test", "Quit?",
			QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::Yes) {
			//QApplication::quit();
		}
		else {

		}
	}
	uint32_t ReturnDataSize();
	void ShowResults (uint64_t nResults)
	{
		_hResult->ui.label_2->setText(ReturnStrFromDecInt(nResults).c_str());
	}
	void IsSetOnTop() {
		if (ui.cbAlwaysOnTop->checkState())
			this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
		else
			this->setWindowFlags(this->windowFlags() ^ Qt::WindowStaysOnTopHint);//^ Qt::WindowStaysOnTopHint
	}
	void FirstScan()
	{
		if (bInScan == 0)
		{
			bInScan = 1;
			ui.comboBValueType->setDisabled(true);
			ui.cbExecutable->setDisabled(true);
			ui.cbWritable->setDisabled(true);
			ui.cbCopyOnWrite->setDisabled(true);
			ui.lineRangeEnd->setDisabled(true);
			ui.lineRangeStart->setDisabled(true);
			ui.pbFirstNewScan->setText("New Scan");
			if (ui.LineScanValue->text() == "")
				return;
			QString text = ui.LineScanValue->text();
			uint64_t a = ui_run_scan(DebuggedProc.mb, 4, text.toInt(), (SEARCH_CONDITION)ui.comboBValueType->currentIndex(), NEW_SCAN);
			ShowResults(a);
		}
		else
		{
			bInScan = 0;
			ui.comboBValueType->setDisabled(false);
			ui.cbExecutable->setDisabled(false);
			ui.cbWritable->setDisabled(false);
			ui.cbCopyOnWrite->setDisabled(false);
			ui.lineRangeEnd->setDisabled(false);
			ui.lineRangeStart->setDisabled(false);
			ui.pbFirstNewScan->setText("First Scan");
		}
	}
	void NextScan()
	{
		if (ui.LineScanValue->text() == "")
			return;
		QString text = ui.LineScanValue->text();
		uint64_t a = ui_run_scan(DebuggedProc.mb, 4 , text.toInt(), COND_EQUALS, NEXT_SCAN); //text.toInt() ui.comboBValueType->currentIndex()
		_hResult->ui.treeWidget->clear();
		if (a < 150)
		print_matches(DebuggedProc.mb, _hResult);
		QTreeWidgetItem * itm = new QTreeWidgetItem(_hResult->ui.treeWidget);
		ShowResults(a);
	}
	SearchWindow(QMainWindow* parent = 0) //: QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() ^ Qt::WindowContextHelpButtonHint);
		this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
		ui.setupUi(this);
		QObject::connect(ui.cbAlwaysOnTop, &QCheckBox::clicked, this, &SearchWindow::IsSetOnTop);
		QObject::connect(ui.pbFirstNewScan, &QPushButton::clicked, this, &SearchWindow::FirstScan);
		QObject::connect(ui.pbNextScan, &QPushButton::clicked, this, &SearchWindow::NextScan);
	}
	~SearchWindow() {}
	void closeEvent(QCloseEvent *event)
	{
		//saves scan settings here
	}
public:
	Ui::Ui_Search ui;
	ResultsWindow* _hResult;
	bool bInScan = 0;
};