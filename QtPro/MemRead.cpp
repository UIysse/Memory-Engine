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
std::fstream fout("Cpp Engine Logs.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
//prototype:
using namespace std;
void fillMemBlocks(MEMBLOCK * mb);

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

MEMBLOCK * QueryMemoryAddrressDriver(int64_t addr)
{
	MEMBLOCK *mb = new MEMBLOCK;
	mb->addr = (unsigned char*)addr;
	mb->buffer = reinterpret_cast <unsigned char *>(malloc(0x1000));
	mb->next = NULL;
	fout << "Memory block was created" << endl;
	// Read();
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