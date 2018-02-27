#pragma once
#include <cstdlib>
#include <Windows.h>
int ProcessPriv();
typedef struct _MEMBLOCK
{
	HANDLE hProc;
	unsigned char *addr;
	unsigned char *allocationBase;
	int size;
	unsigned char *buffer;

	unsigned char *searchmask;
	int matches;
	int data_size;

	struct _MEMBLOCK *next;
} MEMBLOCK;
MEMBLOCK* create_scan(HANDLE procHandle, int data_size);
MEMBLOCK* QueryMemoryAddrress(int64_t addr);
MEMBLOCK* QueryMemoryAddrressDriver(int64_t addr);
void fillEachMemblock(MEMBLOCK * mb);
extern 	std::fstream fout;