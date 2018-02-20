#include "qtpro.h"
#include "AboutCpp.h"
#include "MemoryViewer.h"
#include "OpenProcess.h"
#include "ui_qtpro.h"
#include "ui_about.h"
#include "ui_aboutce.h"
#include "qt_windows.h"
#include "ui_openProcess.h"
#include "MemoryMap.h"
#include "Modules.h"
#include "Search.h"
#include "DebuggedProcess.h"
#include "Logs.h"
#include "MemRead.h"
#include "Header.h"
#include "InputGotoBox.h"
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <string>
#include <memory>
#include <Psapi.h>
#include "ui_ShowDLLs.h"
// ConsoleApplication10.cpp : Defines the entry point for the console application.
//
#define BEA_ENGINE_STATIC  /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL    /* specify the usage of a stdcall version of BeaEngine */
#include "BeaEngine/BeaEngine.h"
#include <fstream> 
#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <iomanip>
#include <sstream>  
#include <cstdio>
#include "../BlackBone/src/BlackBone/DriverControl/DriverControl.h"
#include "test\Header.h"
//#include "../BlackBone/Process/Process.h"
using namespace std;
//#include "../Blackbone/src/BlackBone/Process/Process.h" 

//prototypes
BOOL GetProcessList(QTreeWidget * listwidget);

int insertDisas(MemoryViewer * aDialog)
{
	DebuggedProc.IsHandleValid();
	if (!DebuggedProc.addressOfInterest)
		fout << "error, no EIP used" << endl;

		DebuggedProc.mb = QueryMemoryAddrress(DebuggedProc.addressOfInterest);
	if (DebuggedProc.mb)
	{
		ostringstream stre;
		string str;
		QTreeWidgetItem* itm;
		QFont font("Terminal");
		font.setCapitalization(QFont::AllUppercase);
		font.setPointSize(9);
		font.setStretch(145);
		font.setKerning(false);
		/* ============================= Init datas */
		DISASM MyDisasm;
		int nfalse = 0, ntrue = 1;
		int len, i = 0;
		bool Error = nfalse;

		/* ============================= Init the Disasm structure (important !)*/
		(void)memset(&MyDisasm, 0, sizeof(DISASM));

		/* ============================= Init EIP */
		int64_t n;
		n = reinterpret_cast<int64_t>(DebuggedProc.mb->buffer);
		fout << "Address of Interest : " << hex << DebuggedProc.addressOfInterest << endl;
		fout << "Start of memory page : " << hex << DebuggedProc.basePageAddress << endl;
		fout << "size : " << hex << DebuggedProc.mb->size << endl;
		n = n + DebuggedProc.addressOfInterest - DebuggedProc.basePageAddress;
		int64_t nTargetedProcessAddress = DebuggedProc.addressOfInterest;
		cout << "n : " << hex << n << endl;
		MyDisasm.Archi = DebuggedProc.architecture;
		MyDisasm.EIP = n;// 0x401000;
		MyDisasm.VirtualAddr = DebuggedProc.addressOfInterest;
		cout << "sizeof " << sizeof(MyDisasm.EIP) << endl;
		cout << "start address : " << hex << MyDisasm.EIP << endl;
		uint64_t nTotalBytesDisasembled = 0;
		/* ============================= Loop for Disasm */
		while ((nTotalBytesDisasembled < DebuggedProc.mb->size)) {
			itm = new QTreeWidgetItem(aDialog->ui.treeWidget);
			itm->setFont(2, font); //font is created at the begining of the function
			itm->setFont(0, font);
			itm->setFont(1, font);
			len = Disasm(&MyDisasm);
			string str1;
			int len2;//because len = -1 for unknown opcodes
			if (len > 0)
				len2 = len;
			else
				len2 = 1;
			nTotalBytesDisasembled += len2;
			str1[len2 * 2] = 0;//assigning null terminator to string
			for (int u = 0; u < len2; ++u)
			{
				if (((((*(unsigned char*)(MyDisasm.EIP + u)) >> 4) & 0xF) < 0xA) && ((((*(unsigned char*)(MyDisasm.EIP + u)) >> 4) & 0xF) >= 0))
					str1[u * 2] = (((*(unsigned char*)(MyDisasm.EIP  + u)) >> 4) & 0xF) + '30';
				else
					str1[u * 2] = (((*(unsigned char*)(MyDisasm.EIP  + u)) >> 4) & 0xF) + '37';
				if ((((*(unsigned char*)(MyDisasm.EIP + u)) & 0xF) < 0xA) && (((*(unsigned char*)(MyDisasm.EIP + u)) & 0xF) >= 0))
					str1[u * 2 + 1] = ((*(unsigned char*)(MyDisasm.EIP  + u)) & 0xF) + '30';
				else
					str1[u * 2 + 1] = ((*(unsigned char*)(MyDisasm.EIP  + u)) & 0xF) + '37';
			}
			itm->setText(1, str1.c_str());
			if (len != UNKNOWN_OPCODE) {
				str.clear();
				stre.str("");
				MyDisasm.EIP = MyDisasm.EIP + len;
				MyDisasm.VirtualAddr = MyDisasm.VirtualAddr + len;
				i++;
				stre << std::hex << nTargetedProcessAddress;
				str = stre.str();
				itm->setText(0, str.c_str());
				itm->setText(2, MyDisasm.CompleteInstr);
				//06.03.17 draft color
				string str5(MyDisasm.CompleteInstr);
				auto ddd = str5.find("jn");
				if (ddd != std::string::npos)
				{
					itm->setTextColor(2, Qt::red);
					itm->setBackgroundColor(2, Qt::yellow);
				}
				else if (str5.find("call") != std::string::npos)
				{
					itm->setBackgroundColor(2, Qt::cyan);
				}
				else if (str5.find("jmp") != std::string::npos)
				{
					itm->setBackgroundColor(2, Qt::yellow);
				}
				else
					itm->setTextColor(2, Qt::lightGray);
				nTargetedProcessAddress += len;
			}
			else { //error
				i++;
				Error = false;
				++MyDisasm.EIP;
				++MyDisasm.VirtualAddr;
				str.clear();
				stre.str("");
				stre << std::hex << nTargetedProcessAddress;
				str = stre.str();
				itm->setText(0, str.c_str());
				itm->setText(2, "???");
				++nTargetedProcessAddress;
			}
		};
		LOUT << "total disa : "  << nTotalBytesDisasembled << " block size "  << DebuggedProc.mb->size << endl;
		fout << "finished disasembling." << endl;
	}
	else
		fout << "Disassembling didn't take place because there is no memory block." << endl;
	return 0;
}


void printError(TCHAR* msg);
void QtPro::ShowAboutDialog()
{
	About *w = new About(this);
	w->setModal(true);
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
	
}
void QtPro::UpdateProcessName()
{

}
void QtPro::ShowPickProcess()
{
	//ui.toolButton->setEnabled(false);
	ui.toolButton->setDisabled(true);
	OpenProcessClass* w = new OpenProcessClass(this, this);
	w->setAttribute(Qt::WA_DeleteOnClose);//takes care of releasing memory when the object is closed
	w->show();
	GetProcessList(w->ui.listView);
}
void QtPro::OpenHandle()
{

}
void foo(MemoryViewer * aDialog)
{
	QTreeWidgetItem* itm = new QTreeWidgetItem(aDialog->ui.treeWidget);
	itm->setText(0, "premier text");
	itm->setText(1, "second text");
	itm->setText(2, "troisieme text");
	//ui.treeWidget->addScrollBarWidget(ui.treeWidget, Qt::AlignmentFlag::AlignAbsolute);
}

void QtPro::ShowLogs()
{
	if (pLogsWindow == nullptr)
	{
		pLogsWindow = new Logs(this);
		pLogsWindow->setAttribute(Qt::WA_DeleteOnClose);
		pLogsWindow->show();
	}
	else
	{
		pLogsWindow->show();
		pLogsWindow->activateWindow();
	}
}
void QtPro::ShowSearch()
{
	ResultsWindow * t = new ResultsWindow(this);
	t->setAttribute(Qt::WA_DeleteOnClose);
	t->show();
	SearchWindow * w = new SearchWindow(this);
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
	w->_hResult = &t->ui;
	t->pSearchWindow = &w->ui;
}
void QtPro::ShowModules()
{
	ModulesWindow * w = new ModulesWindow(this);
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
}
void QtPro::ShowThreadList()
{
	ThreadListWindow * w = new ThreadListWindow(this);
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
}
void QtPro::ShowMemoryMap()
{
	MemoryMapWindow* w = new MemoryMapWindow(this);
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
}
QtPro::QtPro(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//setCentralWidget(ui.plainTextEdit);
	QObject::connect(ui.actionAbout, &QAction::triggered, this, &QtPro::ShowAboutDialog);
	QObject::connect(ui.toolButton, &QToolButton::clicked, this, &QtPro::ShowPickProcess);// &QtPro::ShowPickProcess);
	QObject::connect(ui.toolButLogs, &QPushButton::clicked, this, &QtPro::ShowLogs);
	QObject::connect(ui.toolButMod, &QPushButton::clicked, this, &QtPro::ShowModules);
	QObject::connect(ui.toolButSearch, &QPushButton::clicked, this, &QtPro::ShowSearch);
	QObject::connect(ui.toolButMemoryView, &QPushButton::clicked, this, &QtPro::ShowMemoryView);
	QObject::connect(ui.toolButThreads, &QToolButton::clicked, this, &QtPro::ShowThreadList);
	QObject::connect(ui.toolButMemoryMap, &QToolButton::clicked, this, &QtPro::ShowMemoryMap);
}
void QtPro::ShowMemoryView()
{
	MemoryViewer * w = new MemoryViewer(this);
	w->setAttribute(Qt::WA_DeleteOnClose);
	w->show();
}
QtPro::~QtPro()
{
	QApplication::closeAllWindows();
}
void QtPro::closeDialog() // was meant back when QTPro had a member pointer towards About class and called delete ptr here
{

}

void printError(TCHAR* msg)
{
	DWORD eNum;
	TCHAR sysMsg[256];
	TCHAR* p;

	eNum = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, eNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		sysMsg, 256, NULL);

	// Trim the end of the line and terminate it with a null
	p = sysMsg;
	while ((*p > 31) || (*p == 9))
		++p;
	do { *p-- = 0; } while ((p >= sysMsg) &&
		((*p == '.') || (*p < 33)));

	// Display the message
	_tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}
string GetProcNameFromPID(DWORD ParentProc, HANDLE& hProcessSnap)
{
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Now walk the snapshot of processes, and
	// display information about each process in turn
	if (Process32First(hProcessSnap, &pe32))
		do
		{
			if (pe32.th32ProcessID == ParentProc)
			{
				std::wstring ws(pe32.szExeFile);
				std::string str(ws.begin(), ws.end());
				if (str == "[System Process]")
					str = "System's Parent";
				return str;
			}
		} while (Process32Next(hProcessSnap, &pe32));
		string str("Process Terminated");
		return str;
}
BOOL GetProcessList(QTreeWidget * listwidget)
{
	HANDLE hProcessSnap; 
	HANDLE hProcess;
	HANDLE hParentSnap;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;
	// Give maximum rights to our memory viewer
	ProcessPriv();
	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	hParentSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}
	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);
	// Now walk the snapshot of processes, and
	// display information about each process in turn
	if (Process32First(hProcessSnap, &pe32))
		do
	{
		_tprintf(TEXT("\n\n====================================================="));
		_tprintf(TEXT("\nPROCESS NAME:  %s"), pe32.szExeFile);
		_tprintf(TEXT("\n-------------------------------------------------------"));
		std::wstring ws(pe32.szExeFile);
		// your new String
		std::string str(ws.begin(), ws.end());
		//Avoid begin and end meaningless displays
		if (str == "[System Process]")
			continue;
		QTreeWidgetItem * itm = new QTreeWidgetItem(listwidget);
		itm->setIcon(0, QIcon(""));
		itm->setText(1, ReturnStrFromHexaInt(pe32.th32ProcessID).c_str());
		itm->setText(2, QString::fromWCharArray(pe32.szExeFile)); 
		itm->setText(3, GetProcNameFromPID(pe32.th32ParentProcessID, hParentSnap).c_str());
		// Retrieve the priority class.
		dwPriorityClass = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
		{ //Handle access is denied, item will appear red in the list
			itm->setBackgroundColor(2, Qt::red);
		}
		else
		{
			dwPriorityClass = GetPriorityClass(hProcess);
			if (!dwPriorityClass)
				printError(TEXT("GetPriorityClass"));
			CloseHandle(hProcess);
		}
		/*
		_tprintf(TEXT("\n  Process ID        = 0x%08X"), pe32.th32ProcessID);
		_tprintf(TEXT("\n  Thread count      = %d"), pe32.cntThreads);
		_tprintf(TEXT("\n  Parent process ID = 0x%08X"), pe32.th32ParentProcessID);
		_tprintf(TEXT("\n  Priority base     = %d"), pe32.pcPriClassBase);
		if (dwPriorityClass)
			_tprintf(TEXT("\n  Priority class    = %d"), dwPriorityClass);*/

		// List the modules and threads associated with this process
		//ListProcessModules(pe32.th32ProcessID);
		//ListProcessThreads(pe32.th32ProcessID);

		} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(TRUE);
}
HANDLE  ReturnProcessHandle(QString Qstr)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			std::wstring ws(entry.szExeFile);
			// your new String
			std::string str(ws.begin(), ws.end());
			DebuggedProc.wsChosenProc = ws;
			if (stricmp(str.c_str(), Qstr.toStdString().c_str()) == 0)
			{
				//Retrieve target process handle
				DebuggedProc.hwnd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				//Stores the Process Id of the targeted process
				DebuggedProc.targetPid = entry.th32ProcessID;
				if (DebuggedProc.hwnd == 0)
				{
					fout << "Could not open process and get the rights." << endl;
					LOUT << "Could not open process and get the rights." << endl;
				}
				else
				{
					fout << "Process open with all access." << endl;
					LOUT << "Process open with all access." << endl;
				}
				PBOOL pbool = 0;
				BOOL av;
				pbool = &av;
				IsWow64Process(DebuggedProc.hwnd, pbool);
				if (*pbool) // on 64 bit windows, Iswow64Process returns false if process is 64 and true if process is 32
				{
					DebuggedProc.architecture = 0; // 0 for 32 bit : BEA engine doc.
					fout << "32 bit process." << endl;
					LOUT << "32 bit process." << endl;
				}
				else
				{
					DebuggedProc.architecture = 64;
					fout << "64 bit process." << endl;
					LOUT << "64 bit process." << endl;
				}
				if (Read())
				{
					LOUT << "Driver successfully loaded." << endl;
				}
				else
				{
					LOUT << "Error : Driver could not be loaded." << endl;
				}
				HMODULE *modarray = new HMODULE[200];
				DWORD o;
				if (0 == EnumProcessModules(DebuggedProc.hwnd, modarray, sizeof(HMODULE) * 200, &o))
					fout << "Enum Process failed" << endl;
				MODULEINFO  lpmod;
				if (0 == GetModuleInformation(DebuggedProc.hwnd,
					modarray[0],
					&lpmod,
					sizeof(lpmod)
				))
					fout << "GetModuleInformation failed." << endl;
				DebuggedProc.addressOfInterest = reinterpret_cast<int64_t>(lpmod.EntryPoint);
				LOUT << "Module base address : 0x" >> reinterpret_cast<int64_t>(lpmod.lpBaseOfDll) << endl;
				LOUT << "Module entry point : 0x" >> reinterpret_cast<int64_t>(lpmod.EntryPoint) << endl;
				fout << "Entrypoint : " << hex << DebuggedProc.addressOfInterest << " Base address : " << lpmod.lpBaseOfDll << endl;
				return DebuggedProc.hwnd;
			}
		}
	}

	CloseHandle(snapshot);
	return DebuggedProc.hwnd;
}