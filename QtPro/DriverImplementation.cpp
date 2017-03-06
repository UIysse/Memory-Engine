#include "../Blackbone/src/BlackBone/Process/Process.h" 
#include "../Blackbone/src/BlackBone/Config.h"
#include "../Blackbone/src/BlackBone/Process/Process.h"
#include "../Blackbone/src/BlackBone/PE/PEImage.h"
#include "../Blackbone/src/BlackBone/Misc/Utils.h"
#include "../Blackbone/src/BlackBone/Misc/DynImport.h"
#include "DebuggedProcess.h"
#include <Psapi.h>
#include <iostream>
#include <fstream>
#define DRIVOUT driveroutput
using namespace blackbone;
using namespace std;
ofstream driveroutput("driver logs.txt");
wofstream driveroutput2("driver logs2.txt");
Process proc, thisProc; //Global definitions so that the handle remains valid outside Read's scope
bool Read()
{
	
	// proc, thisProc;
	{
	std::vector<DWORD> procIDs;
	Process::EnumByName(DebuggedProc.wsChosenProc, procIDs);
	proc.Attach(procIDs.front());
	thisProc.Attach(GetCurrentProcessId());
	if (NT_SUCCESS(Driver().EnsureLoaded()))
	{
		DRIVOUT << "						#######################   DRIVER LOADED   ##############" << endl;
		NTSTATUS status = STATUS_SUCCESS;
		DWORD depFlags = 0;
		BOOL perm = FALSE;
		uint8_t buf[0x1000] = { 0 };
		MEMORY_BASIC_INFORMATION64 memInfo = { 0 };
		Driver().PromoteHandle(GetCurrentProcessId(), proc.core().handle(), PROCESS_ALL_ACCESS);
		auto address = proc.modules().GetMainModule()->baseAddress;
		ptr_t address2 = 0;
		ptr_t size = 0x1000;
		//
		HMODULE hMods[1024];
		HANDLE hProcess;
		DWORD cbNeeded;
		unsigned int i;
		if (EnumProcessModulesEx(DebuggedProc.hwnd, hMods, sizeof(hMods), &cbNeeded, 0x03))
		{
			DRIVOUT << "cbneeded : " << cbNeeded << endl;
			for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				wchar_t szModName[MAX_PATH];

				// Get the full path to the module's file.

				if (GetModuleFileNameEx(DebuggedProc.hwnd, hMods[i], szModName, MAX_PATH))
				{
					// Print the module name and handle value.

					driveroutput2 << szModName << endl;
				}
			}
		}
		/*
		// Disable DEP
		status = Driver().DisableDEP(GetCurrentProcessId());
		SAFE_CALL(GetProcessDEPPolicy, GetCurrentProcess(), &depFlags, &perm);
		if (!NT_SUCCESS(status) || depFlags & PROCESS_DEP_ENABLE)
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_DISABLE_DEP failed, status 0x" << std::hex << status << "\r\n" << endl;
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_DISABLE_DEP succeeded\r\n" << endl;

		// Make current process protected
		status = Driver().ProtectProcess(GetCurrentProcessId(), true);
		if (!NT_SUCCESS(status) || !thisProc.core().isProtected())
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_SET_PROTECTION failed, status 0x" << std::hex << status << "\r\n" << endl;
		else
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_SET_PROTECTION succeeded\r\n" << endl;
			Driver().ProtectProcess(GetCurrentProcessId(), false);
		}

		// Grant explorer.exe handle full access
		status = Driver().PromoteHandle(GetCurrentProcessId(), proc.core().handle(), PROCESS_ALL_ACCESS);
		if (!NT_SUCCESS(status))
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS failed, status 0x" << std::hex << status << "\r\n" << endl;
		}
		else
		{
			PUBLIC_OBJECT_BASIC_INFORMATION info = { 0 };
			ULONG sz = 0;
			status = SAFE_CALL(NtQueryObject, proc.core().handle(), ObjectBasicInformation, &info, (ULONG)sizeof(info), &sz);
			if (!NT_SUCCESS(status))
			{
				DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS failed, status 0x" << std::hex << status << "\r\n" << endl;
			}
			else
			{
				if (info.GrantedAccess == PROCESS_ALL_ACCESS)
					DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS succeeded\r\n" << endl;
				else
					DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS failed, status 0x" << std::hex << STATUS_UNSUCCESSFUL << "\r\n" << endl;
			}
		}

		// Read explorer.exe PE header
		status = Driver().ReadMem(proc.pid(), address, size, buf);
		if (!NT_SUCCESS(status) || *(uint16_t*)buf != IMAGE_DOS_SIGNATURE)
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_COPY_MEMORY failed, status 0x" << std::hex << status << "\r\n" << endl;
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_COPY_MEMORY succeeded\r\n" << endl;

		// Allocate some memory
		status = Driver().AllocateMem(proc.pid(), address2, size, MEM_COMMIT, PAGE_READWRITE);
		if (!NT_SUCCESS(status) || *(uint16_t*)buf != IMAGE_DOS_SIGNATURE)
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ALLOCATE_FREE_MEMORY failed, status 0x" << std::hex << status << "\r\n" << endl;
		else
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ALLOCATE_FREE_MEMORY succeeded\r\n" << endl;
			Driver().FreeMem(proc.pid(), address2, size, MEM_RELEASE);
		}

		// Make explorer.exe PE header writable
		status = Driver().ProtectMem(proc.pid(), address, 0x1000, PAGE_READWRITE);
		proc.memory().Query(address, &memInfo);

		if (!NT_SUCCESS(status) || !(memInfo.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)))
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_PROTECT_MEMORY failed, status 0x" << std::hex << status << "\r\n" << endl;
		else
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_PROTECT_MEMORY succeeded\r\n" << endl;
			proc.memory().Protect(address, 0x1000, PAGE_READONLY);
		}

		// Unlink handle table
		status = Driver().UnlinkHandleTable(proc.pid());
		if (!NT_SUCCESS(status))
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_UNLINK_HTABLE failed, status 0x" << std::hex << status << "\r\n" << endl;
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_UNLINK_HTABLE succeeded\r\n" << endl;

		// Enum regions
		std::vector<MEMORY_BASIC_INFORMATION64> info;
		status = Driver().EnumMemoryRegions(proc.pid(), info);
		if (!NT_SUCCESS(status))
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ENUM_REGIONS failed, status 0x" << std::hex << status << "\r\n" << endl;
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ENUM_REGIONS succeeded\r\n" << endl;
			*/
		return true;
	}
	else
	{
		DRIVOUT << "failed to load driver" << endl;
		return false;
	}
}
if (WAIT_TIMEOUT == WaitForSingleObject(DebuggedProc.hwnd, 0))
DRIVOUT << "Handle of process is still active" << endl;
else
DRIVOUT << "Handle is dead." << endl;
}
void TestDriver()
{
	Process proc, thisProc;
	std::vector<DWORD> procIDs;
	Process::EnumByName(DebuggedProc.wsChosenProc, procIDs);
	proc.Attach(procIDs.front());
	thisProc.Attach(GetCurrentProcessId());
	if (NT_SUCCESS(Driver().EnsureLoaded()))
	{
		DRIVOUT << "driver loaded" << endl;
		NTSTATUS status = STATUS_SUCCESS;
		DWORD depFlags = 0;
		BOOL perm = FALSE;
		uint8_t buf[0x1000] = { 0 };
		MEMORY_BASIC_INFORMATION64 memInfo = { 0 };
		Driver().PromoteHandle(GetCurrentProcessId(), proc.core().handle(), PROCESS_ALL_ACCESS);
		auto address = proc.modules().GetMainModule()->baseAddress;
		ptr_t address2 = 0;
		ptr_t size = 0x1000;
		// Disable DEP
		status = Driver().DisableDEP(GetCurrentProcessId());
		SAFE_CALL(GetProcessDEPPolicy, GetCurrentProcess(), &depFlags, &perm);
		if (!NT_SUCCESS(status) || depFlags & PROCESS_DEP_ENABLE)
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_DISABLE_DEP failed, status 0x" << std::hex << status << "\r\n";
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_DISABLE_DEP succeeded\r\n";

		// Make current process protected
		status = Driver().ProtectProcess(GetCurrentProcessId(), true);
		if (!NT_SUCCESS(status) || !thisProc.core().isProtected())
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_SET_PROTECTION failed, status 0x" << std::hex << status << "\r\n";
		else
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_SET_PROTECTION succeeded\r\n";
			Driver().ProtectProcess(GetCurrentProcessId(), false);
		}

		// Grant explorer.exe handle full access
		status = Driver().PromoteHandle(GetCurrentProcessId(), proc.core().handle(), PROCESS_ALL_ACCESS);
		if (!NT_SUCCESS(status))
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS failed, status 0x" << std::hex << status << "\r\n";
		}
		else
		{
			PUBLIC_OBJECT_BASIC_INFORMATION info = { 0 };
			ULONG sz = 0;
			status = SAFE_CALL(NtQueryObject, proc.core().handle(), ObjectBasicInformation, &info, (ULONG)sizeof(info), &sz);
			if (!NT_SUCCESS(status))
			{
				DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS failed, status 0x" << std::hex << status << "\r\n";
			}
			else
			{
				if (info.GrantedAccess == PROCESS_ALL_ACCESS)
					DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS succeeded\r\n";
				else
					DRIVOUT << "TestDriver: IOCTL_BLACKBONE_GRANT_ACCESS failed, status 0x" << std::hex << STATUS_UNSUCCESSFUL << "\r\n";
			}
		}

		// Read explorer.exe PE header
		status = Driver().ReadMem(proc.pid(), address, size, buf);
		if (!NT_SUCCESS(status) || *(uint16_t*)buf != IMAGE_DOS_SIGNATURE)
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_COPY_MEMORY failed, status 0x" << std::hex << status << "\r\n";
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_COPY_MEMORY succeeded\r\n";

		// Allocate some memory
		status = Driver().AllocateMem(proc.pid(), address2, size, MEM_COMMIT, PAGE_READWRITE);
		if (!NT_SUCCESS(status) || *(uint16_t*)buf != IMAGE_DOS_SIGNATURE)
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ALLOCATE_FREE_MEMORY failed, status 0x" << std::hex << status << "\r\n";
		else
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ALLOCATE_FREE_MEMORY succeeded\r\n";
			Driver().FreeMem(proc.pid(), address2, size, MEM_RELEASE);
		}

		// Make explorer.exe PE header writable
		status = Driver().ProtectMem(proc.pid(), address, 0x1000, PAGE_READWRITE);
		proc.memory().Query(address, &memInfo);

		if (!NT_SUCCESS(status) || !(memInfo.Protect & (PAGE_READWRITE | PAGE_WRITECOPY)))
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_PROTECT_MEMORY failed, status 0x" << std::hex << status << "\r\n";
		else
		{
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_PROTECT_MEMORY succeeded\r\n";
			proc.memory().Protect(address, 0x1000, PAGE_READONLY);
		}

		// Unlink handle table
		status = Driver().UnlinkHandleTable(proc.pid());
		if (!NT_SUCCESS(status))
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_UNLINK_HTABLE failed, status 0x" << std::hex << status << "\r\n";
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_UNLINK_HTABLE succeeded\r\n";

		// Enum regions
		std::vector<MEMORY_BASIC_INFORMATION64> info;
		status = Driver().EnumMemoryRegions(proc.pid(), info);
		if (!NT_SUCCESS(status))
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ENUM_REGIONS failed, status 0x" << std::hex << status << "\r\n";
		else
			DRIVOUT << "TestDriver: IOCTL_BLACKBONE_ENUM_REGIONS succeeded\r\n";
	}
	else 
		DRIVOUT << "failed to load driver" << endl;
	Driver().Unload();
}