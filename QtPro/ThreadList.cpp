#include "ThreadList.h"
#include "Modules.h"
#include "ui_ThreadList.h"
#include "DebuggedProcess.h"
#include  <Windows.h>
#include <tlhelp32.h>
#include <Winternl.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#define ThreadQuerySetWin32StartAddress 9 //for some reason the winternl.h header doesn't know this value as part of the THREADINFOCLASS enum
enum ThreadPriorities {
	Idle = 1,
	Lowest = 6,
	Normal = 8,
	AboveNormal = 9,
	Highest = 10,
	RealTime = 15,
};

std::string RetThreadPriority(uint32_t nTpriority)
{
	std::string str;
	switch (nTpriority)
	{
	case Idle:
		return str = "Idle";
		break;
	case Lowest:
		return str = "Lowest";
		break;
	case Normal:
		return str = "Normal";
		break;
	case AboveNormal:
		return str = "Above Normal";
		break;
	case Highest:
		return str = "Highest";
		break;
	case RealTime:
		return str = "Real Time";
		break;
	}
	return std::to_string(nTpriority);
}
void ThreadListWindow::UpdateThreadList()
{
	if (!DebuggedProc.IsHandleValid())
		return;
	HANDLE hdl;
	DWORD pId = GetCurrentProcessId();
	THREADENTRY32 te32;
	uint64_t nThreadStartAddress = 0;
	te32.dwSize = sizeof(te32);
	Mods ModMap = UpdateModulesList();
	if ((hdl = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)) == INVALID_HANDLE_VALUE)
		fout << "CreateToolHelp32SnapShot failed." << std::endl;
	if (Thread32First(hdl, &te32))
	{

		do
		{
			if (DebuggedProc.targetPid == te32.th32OwnerProcessID)
			{
				typedef NTSTATUS(WINAPI *pNtQIT)(HANDLE, LONG, PVOID, ULONG, PULONG);
				pNtQIT NtQueryInformationThread = (pNtQIT)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQueryInformationThread");
				HANDLE hwn = OpenThread(THREAD_ALL_ACCESS, false, te32.th32ThreadID);
				NtQueryInformationThread(hwn, (THREADINFOCLASS)ThreadQuerySetWin32StartAddress, (LPVOID)&nThreadStartAddress, sizeof(nThreadStartAddress), NULL);
				QTreeWidgetItem* itm = new QTreeWidgetItem(ui.treeWidget);
				std::stringstream ss;
				ss << std::hex << te32.th32ThreadID;
				std::string str = ss.str();
				itm->setText(0, str.c_str());
				itm->setText(1, RetThreadPriority(te32.tpBasePri).c_str());
				std::stringstream ss1;
				ss1 << std::hex << nThreadStartAddress;
				str = ModMap.FetchModuleName(nThreadStartAddress);
				str += " ";
				str += ss1.str();
				itm->setText(2, str.c_str());
			}
		} while (Thread32Next(hdl, &te32));
		std::string  str = "Thread List : Total ";
		str += std::to_string(ui.treeWidget->topLevelItemCount());
		str += " threads";
		this->setWindowTitle(str.c_str());
	}
	else
		fout << "Thread32First function failed." << std::endl;
}