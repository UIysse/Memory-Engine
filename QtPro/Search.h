#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QMessageBox>
#include "ui_Search.h"
#include "MemoryMap.h"
#include "MemRead.h"
#include "Modules.h"
#include "Results.h"
#include "DebuggedProcess.h"
#include <Windows.h>
#include <winnt.h>
#include <string>
#include <sstream>
#include <MemRead.h>
#define CPPOUT fout
#define WRITABLE_EXECUTE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
#define WRITABLE_NO_EXECUTE (PAGE_READWRITE | PAGE_WRITECOPY)
#define EXECUTE_NO_WRITE (PAGE_EXECUTE_READ | PAGE_EXECUTE)
#define READONLY (PAGE_READONLY)
class MemoryScanner;
class SearchWindow;
typedef enum
{
	NEXT_SCAN,
	NEW_SCAN,
} SCAN_CONDITION;
typedef enum
{
	VALUETYPE_UNKNOWN = 0,
	BYTE_ = 1,
	TWOBYTES_ = 2,
	INT32_ = 3,
	INT64_ = 4,
	FLOAT_ = 5,
	DOUBLE_ = 6,
	STRING_ = 7,
	ArrayBytes_ = 8,
} DATA_SIZE;
enum MemoryScanValueType
{
	VALUETYPE_UNKNOWN2,
	VALUETYPE_BYTE,
	VALUETYPE_2BYTE,
	VALUETYPE_4BYTE,
	VALUETYPE_8BYTE,
	VALUETYPE_FLOAT,
	VALUETYPE_DOUBLE,
	VALUETYPE_STRING,
	VALUETYPE_WSTRING,
	VALUETYPE_AOB
};

// Memory scanner comparison types.
enum MemoryScanType
{
	SCANTYPE_EXACTVALUE,
	SCANTYPE_GREATERTHAN,
	SCANTYPE_SMALLERTHAN,
	SCANTYPE_UNKNOWN_INITIAL_VALUE,
	SCANTYPE_VALUE_IN_BETWEEN,
	SCANTYPE_CHANGED,

	// The same comparetor function applies for the scan types below. Do not add a scan type that requires a different comparetor.
	SCANTYPE_UNCHANGED,
	SCANTYPE_INCREASED,
	SCANTYPE_DECREASED
};
enum SEARCH_CONDITION
{
	COND_EQUALS,
	COND_INCREASED,
	COND_DECREASED,
	COND_UNCONDITIONAL,//unkowninitiavalue
};
// Generalisation struct that can be used to save class instances with template parameter lists.
struct ScanParameterBase
{
	// Indicates whether a fast (aligned) scan should be executed.
	bool CurrentScanFastScan;

	// The type of scan that should be executed.
	//MemoryScanType GlobalScanType;
	SEARCH_CONDITION GlobalScanType;

	// The value type (sizeof) of the value that should be scanned for.
	DATA_SIZE GlobalScanValueType;

	// Are we returning search results in hexadecimal?
	bool CurrentScanHexValues;

	// This parameter contains the size of an array of bytes or the length of a string in case of such a scan.
	// If the scan value type is not one of these, this parameter is ignored.
	unsigned int ValueSize;

	// When searching for strings, it is possible to match strings until null chars are hit. The threshold to
	// avoid CrySearch from reading excessive amounts of memory into a string is 0x100.
	bool ScanUntilNullChar;

	//memory regions parameter
	DWORD AcceptedMemoryState;

	int8_t nValue8;
	int16_t nValue16;
	int32_t nValue32;
	int64_t nValue64;


	//template function inline (class member) which returns the value used to perform the scan as an integer
	//must return either an int64_t either an int32_t
	void GetValue(SearchWindow * pSearchWindow, SCAN_CONDITION NewOrNext);
	// Default constructor should help preventing bogus values.
	uint32_t ScanOffset;
	ScanParameterBase()
	{
		this->CurrentScanFastScan = true;
		this->GlobalScanType = COND_EQUALS;
		this->GlobalScanValueType = VALUETYPE_UNKNOWN;
		this->CurrentScanHexValues = false;
		this->ValueSize = 0;
		this->ScanUntilNullChar = false;
		this->AcceptedMemoryState = 0;
		this->ScanOffset = 1;
	};
	void UpdateSelectedScanOptions(SearchWindow * pSearchWindow);
};
void print_matches(MEMBLOCK *mb_list, Ui_DialogResults* pResultWindow, SearchWindow *pSearchWindow);
int ui_run_scan(MEMBLOCK *scan, uint32_t data_size, int64_t start_val, SEARCH_CONDITION start_cond, SCAN_CONDITION scan_condition);

class SearchWindow : public QDialog
{
	Q_OBJECT
public:
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
	template <typename T>
	int ui_run_scan(MEMBLOCK *scan1, uint32_t data_size, T start_val, SEARCH_CONDITION start_cond, SCAN_CONDITION scan_condition);
	uint32_t ReturnDataSize();
	void ShowResults(uint64_t nResults)
	{
		_hResult->label_2->setText(ReturnStrFromDecInt(nResults).c_str());
	}
	bool FilterRegions(MEMORY_BASIC_INFORMATION &meminfo)
	{
		if (ui.cbWritable->checkState() == Qt::PartiallyChecked)
			fout << "writable partially checked" << std::endl;
		else if (ui.cbWritable->checkState() == Qt::Checked)
			fout << "writable checked" << std::endl;
		else if (ui.cbWritable->checkState() == Qt::Unchecked)
			fout << "writable unchecked" << std::endl;


		if ((meminfo.State & MEM_COMMIT) && (meminfo.Protect & WRITABLE_EXECUTE))
			return true;
		else
			return false;
	}
	void IsSetOnTop() {
		if (ui.cbAlwaysOnTop->checkState())
		{
			this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
			this->show();
		}
		else
		{
			this->setWindowFlags(this->windowFlags() ^ Qt::WindowStaysOnTopHint);
			this->show();
		}
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
			pScanOptions->UpdateSelectedScanOptions(this);
			if (pScanOptions->GlobalScanType != COND_UNCONDITIONAL)
			{
				QString text = ui.LineScanValue->text();
				if (text == "")//This check should only happen when we try to scan
				{
					QMessageBox::warning(this, "Error", "Enter a value", QMessageBox::Ok); //a value must be filled in for the scan to go through
					return;
				}
				std::string strString = text.toStdString();
				if (!std::all_of(strString.begin(), strString.end(), ::isdigit))
					if (!ui.cbHex->isChecked())//asserts we have no hex number when scanning with hex unchecked
					{
						QMessageBox::warning(this, "Invalid number", "This isn't a decimal number.", QMessageBox::Ok);
					}
			}
			this->ModMap = new Mods;
			*ModMap = UpdateModulesList();
			pScanOptions->GetValue(this, NEW_SCAN);
			//different calls according to value size
			if (nResults < 150)
				print_matches(DebuggedProc.mb, _hResult, this);
			ShowResults(nResults);
			if (ui.comboBScanType->currentIndex() == 3)
				ui.comboBScanType->setCurrentIndex(0);
			ui.pbNextScan->setDisabled(false);
		}
		else
		{
			bInScan = 0;
			nResults = 0;
			ui.comboBValueType->setDisabled(false);
			ui.cbExecutable->setDisabled(false);
			ui.cbWritable->setDisabled(false);
			ui.cbCopyOnWrite->setDisabled(false);
			ui.lineRangeEnd->setDisabled(false);
			ui.lineRangeStart->setDisabled(false);
			ui.pbFirstNewScan->setText("First Scan");
			ui.pbNextScan->setDisabled(true);
			_hResult->treeWidget->clear();
			ShowResults(nResults);
			delete ModMap;
			this->ModMap = nullptr;
		}
	}
	void NextScan()
	{
		pScanOptions->UpdateSelectedScanOptions(this);
		if (pScanOptions->GlobalScanType == COND_EQUALS)
		{
			QString text = ui.LineScanValue->text();
			if (text == "")//This check should only happen when we try to scan
			{
				QMessageBox::warning(this, "Error", "Enter a value", QMessageBox::Ok); //a value must be filled in for the scan to go through
				return;
			}
			std::string strString = text.toStdString();
			if (!std::all_of(strString.begin(), strString.end(), ::isdigit))
				if (!ui.cbHex->isChecked())//asserts we have no hex number when scanning with hex unchecked
				{
					QMessageBox::warning(this, "Invalid number", "This isn't a decimal number.", QMessageBox::Ok);
				}
		}
		pScanOptions->GetValue(this, NEXT_SCAN);
		_hResult->treeWidget->clear();
		if (nResults < 150)
			print_matches(DebuggedProc.mb, _hResult, this);
		ShowResults(nResults);
	}
	SearchWindow(QMainWindow* parent = 0) //: QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() ^ Qt::WindowContextHelpButtonHint);
		this->setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
		ui.setupUi(this);
		QObject::connect(ui.cbAlwaysOnTop, &QCheckBox::clicked, this, &SearchWindow::IsSetOnTop);
		QObject::connect(ui.pbFirstNewScan, &QPushButton::clicked, this, &SearchWindow::FirstScan);
		QObject::connect(ui.pbNextScan, &QPushButton::clicked, this, &SearchWindow::NextScan);
		pScanOptions = new ScanParameterBase();
	}
	~SearchWindow() {}
	void closeEvent(QCloseEvent *event)
	{
		//saves scan settings here
	}
public:
	Ui::Ui_Search ui;
	//ResultsWindow *_hResult;
	Ui_DialogResults *_hResult;
	ScanParameterBase * pScanOptions;
	MEMBLOCK *scan;
	Mods *ModMap;
	bool bInScan = 0;
	uint64_t nResults;
};