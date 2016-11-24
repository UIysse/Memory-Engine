#include "qtpro.h"
#include "ui_ShowDLLs.h"

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "MemRead.h"
#include "DebuggedProcess.h"
#include "Header.h"
#define IS_IN_SEARCH(mb,offset) (mb->searchmask[(offset)/8] & (1<<((offset)%8)))
#define REMOVE_FROM_SEARCH(mb,offset) mb->searchmask[(offset)/8] &= ~(1<<((offset)%8));
std::fstream fout("Cpp Engine Logs.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
//prototype:
using namespace std;
void fillMemBlocks(MEMBLOCK * mb);
typedef enum
{
	COND_UNCONDITIONAL,
	COND_EQUALS,

	COND_INCREASED,
	COND_DECREASED,
} SEARCH_CONDITION;


// Enable or disable a privilege in an access token
// source: http://msdn.microsoft.com/en-us/library/aa446619(VS.85).aspx
BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),   
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}

MEMBLOCK* create_memblock(HANDLE hProc, MEMORY_BASIC_INFORMATION *meminfo, int data_size)
{
	MEMBLOCK *mb = new MEMBLOCK; //reinterpret_cast <MEMBLOCK *>(malloc(sizeof(MEMBLOCK)));
	if (mb)
	{
		mb->hProc = hProc;
		mb->addr = reinterpret_cast <unsigned char *>(meminfo->BaseAddress);
		mb->size = meminfo->RegionSize;
		mb->buffer = reinterpret_cast <unsigned char *>(malloc(meminfo->RegionSize));
		mb->searchmask = reinterpret_cast <unsigned char *>(malloc(meminfo->RegionSize / 8));
		memset(mb->searchmask, 0xff, meminfo->RegionSize / 8);
		mb->matches = meminfo->RegionSize;
		mb->data_size = data_size;
		mb->next = NULL;
		fout << "Memory block was created" << endl;
	}
	else
		fout << "Memory block couldn't be created" << endl;

	return mb;
}

MEMBLOCK * QueryMemoryAddrressDriver(int64_t addr)
{
	MEMBLOCK *mb = new MEMBLOCK;
	mb->addr = (unsigned char*)addr;
	mb->buffer = reinterpret_cast <unsigned char *>(malloc(0x1000));
	mb->next = NULL;
	fout << "Memory block was created" << endl;
	 Read();
	return mb;
}
MEMBLOCK * QueryMemoryAddrress(int64_t addr)
{
	MEMORY_BASIC_INFORMATION meminfo;
	MEMBLOCK *mb = nullptr;
	DWORD error = GetLastError();
	if (DebuggedProc.hwnd)
	{
			if (VirtualQueryEx(DebuggedProc.hwnd, (LPCVOID)addr, &meminfo, sizeof(meminfo)) == 0)
			{
				fout << "Virtual Query failed" << endl;
			}
#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ)
			if ((meminfo.State & MEM_COMMIT))// && (meminfo.Protect & WRITABLE))
			{
				fout << "Memory pages are writable and commited" << endl;
				mb = create_memblock(DebuggedProc.hwnd, &meminfo, 1);
				fout << "base add : " << hex << meminfo.BaseAddress << endl;
				fout << "wanted add : " << hex << addr << endl;
				fout << "regionsize : " << meminfo.RegionSize << endl;
				addr += meminfo.RegionSize;
			}
			else
			{
				fout << "Memory pages are not writable and/or not MEM comit" << endl;
			}
	}
	else
		fout << "no process handle" << endl;
	DebuggedProc.basePageAddress =(int64_t) meminfo.BaseAddress;
	fillEachMemblock(mb);
	return mb;
}

void fillEachMemblock(MEMBLOCK * mb)
{
	static unsigned char tempbuf[128 * 1024];
	unsigned int bytes_left;
	unsigned int total_read;
	unsigned int bytes_to_read;
	if (mb)
	{
		uint64_t bytes_read;
		bytes_left = mb->size;
		total_read = 0;
		fout << "mb.size : " << mb->size << endl;
		while (bytes_left)
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			if (0 == ReadProcessMemory(DebuggedProc.hwnd, mb->addr + total_read, (mb->buffer) + total_read, bytes_to_read, &bytes_read))
				fout << "could not read any memory" << endl;
			else
				fout << "could read memory" << endl;
			if (bytes_read != bytes_to_read)
			{
				fout << "Can't read more bytes" << endl;
				break;
			}

			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
	}
	else
		fout << "No memory block" << endl;
	fout << "Bytes left : " << hex << bytes_left << endl;
	fout << "Total bytes read : " << hex << total_read << endl;
}
void fillMemBlocks(MEMBLOCK * mb)
{
	while (mb)
	{
		fillEachMemblock(mb);
		mb = mb->next;
	}
}





unsigned int str2int(char *s)
{
	int base = 10;

	if (s[0] == '0' && s[1] == 'x')
	{
		base = 16;
		s += 2;
	}

	return strtoul(s, NULL, base);
}





int ProcessPriv()
{
	// get process handle
	HANDLE hProc = GetCurrentProcess();

	// get access token of process
	HANDLE hToken = NULL;
	if (!OpenProcessToken(hProc, TOKEN_ADJUST_PRIVILEGES, &hToken))
		fout << "Failed to open access token" << endl;

	// set token privileges to SE_DEBUG_NAME to able to access OpenProcess() with PROCESS_ALL_ACCESS
	if (!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
		fout <<"Failed to set debug privilege" << endl;

	return 0;
}