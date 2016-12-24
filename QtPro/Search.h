#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QMessageBox>
#include "ui_Search.h"
#include "MemoryMap.h"
#include "MemRead.h"
#include "DebuggedProcess.h"
#include <Windows.h>
#include <winnt.h>
#include <string>
#include <sstream>
#define WRITABLE_EXECUTE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
#define WRITABLE_NO_EXECUTE (PAGE_READWRITE | PAGE_WRITECOPY)
#define EXECUTE_NO_WRITE (PAGE_EXECUTE_READ | PAGE_EXECUTE)
#define READONLY (PAGE_READONLY)
class ResultsWindow;
class MemoryScanner;
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
typedef enum SEARCH_CONDITION
{
	COND_EQUALS,
	COND_INCREASED,
	COND_DECREASED,
	COND_UNCONDITIONAL,//unkowninitiavalue
};
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
	int ui_run_scan(MEMBLOCK *scan1, uint32_t data_size, int64_t start_val, SEARCH_CONDITION start_cond, SCAN_CONDITION scan_condition);
	uint32_t ReturnDataSize();
	void ShowResults (uint64_t nResults)
	{
		_hResult->ui.label_2->setText(ReturnStrFromDecInt(nResults).c_str());
	}
	bool FilterRegions (MEMORY_BASIC_INFORMATION meminfo)
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
		uint64_t a;
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
			if (ui.cbHex->isChecked())
			{
				a = ui_run_scan(DebuggedProc.mb, ReturnDataSize(), text.toInt(0, 16), (SEARCH_CONDITION)ui.comboBScanType->currentIndex(), NEW_SCAN);
			}
			else
			{
				a = ui_run_scan(DebuggedProc.mb, ReturnDataSize(), text.toInt(0, 10), (SEARCH_CONDITION)ui.comboBScanType->currentIndex(), NEW_SCAN);
			}
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
		uint64_t a = 0;
		if (ui.cbHex->isChecked())
		{
			a = ui_run_scan(DebuggedProc.mb, ReturnDataSize(), text.toInt(0, 16), (SEARCH_CONDITION)ui.comboBScanType->currentIndex(), NEXT_SCAN);
		}
		else
		{
			a = ui_run_scan(DebuggedProc.mb, ReturnDataSize(), text.toInt(0, 10), (SEARCH_CONDITION)ui.comboBScanType->currentIndex(), NEXT_SCAN);
		}
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
	MEMBLOCK *scan;
	bool bInScan = 0;
};


enum MemoryScanValueType
{
	VALUETYPE_UNKNOWN,
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
	SCANTYPE_UNKNOWN_INITIAL_VALUE,
	SCANTYPE_EXACTVALUE,
	SCANTYPE_SMALLERTHAN,
	SCANTYPE_GREATERTHAN,
	SCANTYPE_VALUE_IN_BETWEEN,
	SCANTYPE_CHANGED,

	// The same comparetor function applies for the scan types below. Do not add a scan type that requires a different comparetor.
	SCANTYPE_UNCHANGED,
	SCANTYPE_INCREASED,
	SCANTYPE_DECREASED
};

// Generalisation struct that can be used to save class instances with template parameter lists.
struct ScanParameterBase
{
	// Indicates whether a fast (aligned) scan should be executed.
	bool CurrentScanFastScan;

	// The type of scan that should be executed.
	MemoryScanType GlobalScanType;

	// The value type (sizeof) of the value that should be scanned for.
	MemoryScanValueType GlobalScanValueType;

	// Are we returning search results in hexadecimal?
	bool CurrentScanHexValues;

	// This parameter contains the size of an array of bytes or the length of a string in case of such a scan.
	// If the scan value type is not one of these, this parameter is ignored.
	unsigned int ValueSize;

	// When searching for strings, it is possible to match strings until null chars are hit. The threshold to
	// avoid CrySearch from reading excessive amounts of memory into a string is 0x100.
	bool ScanUntilNullChar;

	// Default constructor should help preventing bogus values.
	ScanParameterBase()
	{
		this->CurrentScanFastScan = true;
		this->GlobalScanType = SCANTYPE_UNKNOWN_INITIAL_VALUE;
		this->GlobalScanValueType = VALUETYPE_UNKNOWN;
		this->CurrentScanHexValues = false;
		this->ValueSize = 0;
		this->ScanUntilNullChar = false;
	};
};

// Scan parameters given to a new scan.
template <typename T>
struct ScanParameters : public ScanParameterBase
{
	// The value to scan a process' memory for. If the user wants to scan for a value that is
	// in between of two values, this variable is the first value of the two.
	T ScanValue;

	// If the user wants tot scan for a value that is in between of two values, this variable is
	// the second of the two. Otherwise, this variable is not used.
	T OuterScanValue;
};

// Global variable to store scan parameters because of the performance issues a function parameter brings in.
extern ScanParameterBase* GlobalScanParameter;

// Compare functions are listed below.

template <typename T>
const bool CompareEqual(const T& input, const T& expected);

template <typename T>
const bool CompareSmaller(const T& input, const T& expected);

template <typename T>
const bool CompareGreater(const T& input, const T& expected);

template <typename T>
const bool CompareInBetween(const T& input, const T& expected);

template <typename T>
const bool CompareUnknownInitialValue(const T& input, const T& expected);

// Compare function type definition.
template <typename T>
using CompareFunctionType = const bool(*)(const T& input, const T& expected);


// Function that assigns the correct compare function using the user selected scan type, and fires of the workers accordingly.
template <typename T>
void MemoryScanner::AssignAndFire(const bool first)
{
	// Assign the workers the correct compare function.
	CompareFunctionType<T> cmpFunc;
	switch (GlobalScanParameter->GlobalScanType)
	{
	case SCANTYPE_UNKNOWN_INITIAL_VALUE:
		cmpFunc = CompareUnknownInitialValue;
		break;
	case SCANTYPE_EXACTVALUE:
		cmpFunc = CompareEqual;
		break;
	case SCANTYPE_SMALLERTHAN:
		cmpFunc = CompareSmaller;
		break;
	case SCANTYPE_GREATERTHAN:
		cmpFunc = CompareGreater;
		break;
	case SCANTYPE_VALUE_IN_BETWEEN:
		cmpFunc = CompareInBetween;
		break;
	case SCANTYPE_CHANGED:
		cmpFunc = CompareEqual;
		break;
	case SCANTYPE_UNCHANGED:
		cmpFunc = CompareEqual;
		break;
	case SCANTYPE_INCREASED:
		cmpFunc = CompareGreater;
		break;
	case SCANTYPE_DECREASED:
		cmpFunc = CompareSmaller;
		break;
	default:
		cmpFunc = NULL;
		break;
	}
}
// The memory scanning class. Used for main memory scanning and reading.
class MemoryScanner
{
private:
	// Information about the opened process.
	HANDLE mOpenedProcessHandle;
	int mLoadedProcessId;
	bool isX86Process;
	bool mProcessSuspended;

	// Memory scanner control variables.
	bool ScanRunning;

	typedef MemoryScanner CLASSNAME;

	// Function that assigns the correct compare function using the user selected scan type, and fires of the workers accordingly.
	template <typename T>
	void AssignAndFire(const bool first);
	/*
	// Templated worker function for the first scans.
	template <typename T>
	void FirstScanWorker(WorkerRegionParameterData* const regionData, const T& value, CompareFunctionType<T> cmp);

	// Templated worker function for the refresh scans.
	template <typename T>
	void NextScanWorker(WorkerRegionParameterData* const regionData, const T& value, CompareFunctionType<T> cmp);
	*/
	// Singleton code: private constructor, destructor and copy constructors.
	MemoryScanner();
	~MemoryScanner();

	//MemoryScanner(MemoryScanner const&);
	//void operator=(MemoryScanner const&);
public:
	static MemoryScanner* GetInstance()
	{
		static MemoryScanner instance;
		return &instance;
	}


	template <typename T>
	void FirstScan();

	template <typename T>
	void NextScan();

	template <typename T>
	void Poke(const SIZE_T address, const T& value);

	// Size parameter is optional. If AOB or String types are used, parameter is used, otherwise ignored.
	template <typename T>
	bool Peek(const SIZE_T address, const unsigned int size, T* outBuffer) const;
};