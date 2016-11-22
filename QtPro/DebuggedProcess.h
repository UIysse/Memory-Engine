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
#define DRIVOUT driveroutput
class DebuggedProcess
{
public :
	PROCESSENTRY32 entry;
	HANDLE hwnd;
	MEMBLOCK* mb;
	int architecture = 0;
	bool bDriver = 1;
	int64_t addressOfInterest;
	int64_t basePageAddress;
	std::wstring wsChosenProc;
};
extern DebuggedProcess DebuggedProc;
extern std::ofstream driveroutput;