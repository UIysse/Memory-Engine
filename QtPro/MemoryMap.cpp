#include "MemoryMap.h"
#include "DebuggedProcess.h"
#include <Windows.h>
#include <QtWidgets/QTreeWidget>
#include <string>
#include <sstream>
#include <cstdlib>
#define PAGE_TARGETS_INVALID 0x40000000
#define PAGE_TARGETS_NO_UPDATE 0x40000000
std::string ReturnStrFromHexaInt(int64_t arg);
std::string ReturnStrAllocProtect(DWORD allocProtec)
{
	switch (allocProtec)
	{
	case PAGE_EXECUTE:
		return "E----";
	case PAGE_EXECUTE_READ:
		return "ER---";
	case PAGE_EXECUTE_READWRITE:
		return "ERW--";
	case PAGE_EXECUTE_WRITECOPY:
		return "ERWC-";
	case PAGE_NOACCESS:
		return "";
	case PAGE_READONLY:
		return "-R---";
	case PAGE_READWRITE:
		return "-RW--";
	case PAGE_WRITECOPY:
		return "--WC-";
	case PAGE_TARGETS_INVALID:
		return "invalid";
	}
	return ReturnStrFromHexaInt(allocProtec);
}
std::string ReturnStrMemState(DWORD MemoryState)
{
	switch (MemoryState)
	{
	case MEM_COMMIT:
		return "Commit";
	case MEM_FREE:
		return "Free";
	case MEM_RESERVE:
		return "Reserve";
	}
	return ReturnStrFromHexaInt(MemoryState);
}
std::string ReturnStrFromHexaInt(int64_t arg) {
	std::stringstream ss;
	std::string str;
	ss << std::hex << arg;
	return str = ss.str();
}
std::string ReturnStrFromDecInt(int64_t arg) {
	std::stringstream ss;
	std::string str;
	ss << std::dec << arg;
	return str = ss.str();
}
std::string ReturnStrMemType(DWORD MemoryType)
{
	switch (MemoryType)
	{
	case MEM_IMAGE:
		return "IMG";
	case MEM_MAPPED:
		return "MAP";
	case MEM_PRIVATE:
		return "PRV";
	}
	return ReturnStrFromHexaInt(MemoryType);
}

void MemoryMapWindow::UpdateMemoryMap() {
	MEMORY_BASIC_INFORMATION meminfo;
	meminfo.BaseAddress = 0;
	uint64_t addr = 0;
	for (uint64_t uuu = 0; uuu < 5000; ++uuu)
	{
		if (VirtualQueryEx(DebuggedProc.hwnd, (LPVOID)addr, &meminfo, sizeof(meminfo)) == 0)
			break;
		addr = meminfo.RegionSize + (uint64_t)meminfo.BaseAddress;
		if (ReturnStrMemState(meminfo.State) == "Free")
			continue;
		QTreeWidgetItem * itm = new QTreeWidgetItem(ui.treeWidget);
		itm->setText(0, ReturnStrFromHexaInt((uint64_t)meminfo.BaseAddress).c_str());
		itm->setText(2, ReturnStrFromHexaInt(meminfo.RegionSize).c_str()); 
		itm->setText(3, ReturnStrFromHexaInt((uint64_t)meminfo.AllocationBase).c_str());
		itm->setText(6, ReturnStrMemType(meminfo.Type).c_str());
		itm->setText(7, ReturnStrAllocProtect(meminfo.Protect).c_str());
		itm->setText(8, ReturnStrAllocProtect(meminfo.AllocationProtect).c_str());
		itm->setText(1, ReturnStrMemState(meminfo.State).c_str());
	}
}