#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tlhelp32.h>
#include <cstdlib>
#include "MemRead.h"
#include "windows.h"
#include "LogsOutput.h"
#define DRIVOUT driveroutput
void WriteTarget(int value, int address, int size);
class DebuggedProcess
{
public :
	PROCESSENTRY32 entry;
	HANDLE hwnd;
	DWORD targetPid; 
	MEMBLOCK* mb;
	int architecture = 0;
	bool bDriver = 1;
	int64_t addressOfInterest;
	int64_t basePageAddress;
	std::wstring wsChosenProc;
	//windows of CppEngine
	LogsOutput *pLogsOutput;
	//functions
	DebuggedProcess() : pLogsOutput(nullptr) {
	}
	~DebuggedProcess()
	{

	}
	bool IsHandleValid() {
		if (WAIT_TIMEOUT == WaitForSingleObject(this->hwnd, 0))
		{
			fout << "Handle of process is still active" << std::endl;
			return true;
		}
		else
		{
			fout << "Handle is dead." << std::endl;
			return false;
		}
	}
};
extern DebuggedProcess DebuggedProc;
extern std::ofstream driveroutput;