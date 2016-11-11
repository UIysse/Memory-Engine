#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>
#include <cstdlib>
#include "MemRead.h"
#include "windows.h"
class DebuggedProcess
{
public :
	PROCESSENTRY32 entry;
	HANDLE hwnd;
	MEMBLOCK* mb;
	int architecture = 0;
	int64_t addressOfInterest;
	int64_t basePageAddress;
};
extern DebuggedProcess DebuggedProc;