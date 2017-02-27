#include "DebuggedProcess.h"

DebuggedProcess DebuggedProc;
void WriteTarget(int value, int address, int size)
{
	WriteProcessMemory(DebuggedProc.hwnd, (LPVOID)address, &value, size, NULL);
}