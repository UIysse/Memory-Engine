#include "DebuggedProcess.h"

DebuggedProcess DebuggedProc;
bool WriteTarget(int value, int address, int size)
{
	DRIVOUT << "value " << value << "addr " << address << "size " << size << std::endl;
	return WriteProcessMemory(DebuggedProc.hwnd, (LPVOID)address, (LPCVOID)value, size, NULL);
}