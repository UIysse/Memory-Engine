
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include "CheckPerformance.h"
#include "DebuggedProcess.h"
#include "Modules.h"
#include "MemRead.h"
#include "Search.h"
#include "MyMutexes.h"
#include <thread>
#include <vector>
#define CPPOUT fout //(*(DebuggedProc.pLogsOutput))
#define IS_IN_SEARCH(mb, offset) (mb->searchmask[(offset)/8] & (1<<((offset)%8)))
#define REMOVE_FROM_SEARCH(mb, offset)  mb->searchmask[(offset)/8] &= ~(1<<((offset)%8));

inline MEMBLOCK* create_memblock(HANDLE hProc, MEMORY_BASIC_INFORMATION *meminfo, int data_size)
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
	}
	return mb;
}

void free_memblock(MEMBLOCK *mb)
{
	if (mb)
	{
		if (mb->buffer)
		{
			free(mb->buffer);
		}

		if (mb->searchmask)
		{
			free(mb->searchmask);
		}

		free(mb);
	}
}
template <typename T>
inline void update_memblockEqual(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	//CPPOUT << "update memblockEqual" << std::endl;
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	//CPPOUT << "val : " << val1 << " size : " << mb->data_size << std::endl;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
	int bytes_left; //leave these as signed ints
	int total_read;
	int bytes_to_read;
	int64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		//CPPOUT << "Start address scan this block : "<< hex << mb->addr << std::endl;
		while ((bytes_left > sizeof(T)))
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, (uint64_t*)&bytes_read);
			
			if (bytes_read != bytes_to_read) //This block is only entered if page is guarded (or gets unallocated) in which case mb->size = sizeof(T) which yields a false positive on first address of block
			{
				//gotta treat this for potential deallocation
				//CPPOUT << "address read " <<  std::hex << reinterpret_cast<int>( mb->addr + total_read) << std::endl;
				break; 
			}
				unsigned int offset;
				for (offset = 0; offset + sizeof(T) -1 < bytes_read; offset += nOffsetUnit)//; offset < (bytes_read - sizeof(T) + 1);
				{
					//if (IS_IN_SEARCH(mb, (total_read + offset)))
					{
						temp_val = *((T*)&tempbuf[offset]);//significant performance hit if comparing the pointed value to val (instead of assigning it before compare)
						if (temp_val == val)
						{
							++nMatches;
						}
						else
						{
							REMOVE_FROM_SEARCH(mb, (total_read + offset));
						}

					}
				}
				memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
				bytes_read -= (sizeof(T));//will move sizeof(T) more bytes every read than necessary, but must be here to avoid having bogus value for the last bytes of the mem block
				bytes_left -= bytes_read;
				total_read += bytes_read;
		}
			for (int uoffset = 1; uoffset < sizeof(T); ++uoffset)
				REMOVE_FROM_SEARCH(mb, (total_read + uoffset));
		mb->matches = nMatches;
		cSearchWindint->pScanOptions->TotalBytesRead += (total_read + sizeof(T));
	}
}
template <>
inline void update_memblockEqual(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, float val)
{
	typedef double T;
	//CPPOUT << "update memblockEqual" << std::endl;
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	LOUT << "scan float" << endl;
	//CPPOUT << "val : " << val1 << " size : " << mb->data_size << std::endl;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
	int bytes_left; //leave these as signed ints
	int total_read;
	int bytes_to_read;
	int64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		//CPPOUT << "Start address scan this block : "<< hex << mb->addr << std::endl;
		while ((bytes_left > sizeof(T)))
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, (uint64_t*)&bytes_read);

			if (bytes_read != bytes_to_read) //This block is only entered if page is guarded (or gets unallocated) in which case mb->size = sizeof(T) which yields a false positive on first address of block
			{
				//gotta treat this for potential deallocation
				//CPPOUT << "address read " <<  std::hex << reinterpret_cast<int>( mb->addr + total_read) << std::endl;
				break;
			}
			unsigned int offset;
			for (offset = 0; offset + sizeof(T) - 1 < bytes_read; offset += nOffsetUnit)//; offset < (bytes_read - sizeof(T) + 1);
			{
				//if (IS_IN_SEARCH(mb, (total_read + offset)))
				{
					temp_val = *((T*)&tempbuf[offset]);//significant performance hit if comparing the pointed value to val (instead of assigning it before compare)
					if (temp_val == val)
					{
						++nMatches;
					}
					else
					{
						REMOVE_FROM_SEARCH(mb, (total_read + offset));
					}

				}
			}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_read -= (sizeof(T));//will move sizeof(T) more bytes every read than necessary, but must be here to avoid having bogus value for the last bytes of the mem block
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
		for (int uoffset = 1; uoffset < sizeof(T); ++uoffset)
			REMOVE_FROM_SEARCH(mb, (total_read + uoffset));
		mb->matches = nMatches;
		cSearchWindint->pScanOptions->TotalBytesRead += (total_read + sizeof(T));
	}
}
template <typename T>
inline void update_memblockEqualNextScan(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	//CPPOUT << "update memblockEqual" << std::endl;
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	//CPPOUT << "val : " << val1 << " size : " << mb->data_size << std::endl;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
	int bytes_left; //leave these as signed ints
	int total_read;
	int bytes_to_read;
	int64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		while ((bytes_left > sizeof(T)))
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, (uint64_t*)&bytes_read);

			if (bytes_read != bytes_to_read) //This block is only entered if page is guarded (or gets unallocated) in which case mb->size = sizeof(T) which yields a false positive on first address of block
			{
				CPPOUT << "test03 " << std::endl;
				fout << "address read " << std::hex << reinterpret_cast<int>(mb->addr + total_read) << std::endl;
				break;
			}
			unsigned int offset;
			for (offset = 0; offset + sizeof(T) - 1 < bytes_read; offset += nOffsetUnit)//; offset < (bytes_read - sizeof(T) + 1);
			{
				if (IS_IN_SEARCH(mb, (total_read + offset)))
				{
					temp_val = *((T*)&tempbuf[offset]);//significant performance hit if comparing the pointed value to val (instead of assigning it before compare)
					if (temp_val == val)
					{
						++nMatches;
					}
					else
					{
						REMOVE_FROM_SEARCH(mb, (total_read + offset));
					}

				}
			}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_read -= (sizeof(T));//will move sizeof(T) more bytes every read than necessary, but must be here to avoid having bogus value for the last bytes of the mem block
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
		if (bytes_read == 0)
		{//in this block of code, the page is guarded or got unallocated
			CPPOUT << "got executed" << std::endl;
			mb->size = 0;
			mb->matches = nMatches;
			return;
		}
		for (int uoffset = 1; uoffset < sizeof(T); ++uoffset)
			REMOVE_FROM_SEARCH(mb, (total_read + uoffset));
		mb->size = total_read + sizeof(T);
		mb->matches = nMatches;
		cSearchWindint->pScanOptions->TotalBytesRead += (total_read + sizeof(T));
	}
}
template <typename T>
inline void update_memblockChanged(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
	int bytes_left; //leave these as signed ints
	int total_read;
	int bytes_to_read;
	int64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		while ((bytes_left > sizeof(T)))
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, (uint64_t*)&bytes_read);

			if (bytes_read != bytes_to_read) //This block is only entered if page is guarded (or gets unallocated) in which case mb->size = sizeof(T) which yields a false positive on first address of block
			{
				break;
			}
			unsigned int offset;
			for (offset = 0; offset + sizeof(T) - 1 < bytes_read; offset += nOffsetUnit)//; offset < (bytes_read - sizeof(T) + 1);
			{
				if (IS_IN_SEARCH(mb, (total_read + offset)))
				{
						temp_val = *((T*)&tempbuf[offset]);
						prev_val = *((T*)&mb->buffer[total_read + offset]);
					if (temp_val != prev_val)
					{
						++nMatches;
					}
					else
					{
						REMOVE_FROM_SEARCH(mb, (total_read + offset));
					}

				}
			}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_read -= (sizeof(T));//will move sizeof(T) more bytes every read than necessary, but must be here to avoid having bogus value for the last bytes of the mem block
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
		for (int uoffset = 1; uoffset < sizeof(T); ++uoffset) // bug if scan for 1 byte will leave out last byte of memory block
			REMOVE_FROM_SEARCH(mb, (total_read + uoffset));
		mb->size = total_read + sizeof(T);
		mb->matches = nMatches;
		cSearchWindint->pScanOptions->TotalBytesRead += (total_read + sizeof(T));
	}
}
template <typename T>
inline void update_memblockUnChanged(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
	int bytes_left; //leave these as signed ints
	int total_read;
	int bytes_to_read;
	int64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		while ((bytes_left > sizeof(T)))
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, (uint64_t*)&bytes_read);

			if (bytes_read != bytes_to_read) //This block is only entered if page is guarded (or gets unallocated) in which case mb->size = sizeof(T) which yields a false positive on first address of block
			{
				break;
			}
			unsigned int offset;
			for (offset = 0; offset + sizeof(T) - 1 < bytes_read; offset += nOffsetUnit)//; offset < (bytes_read - sizeof(T) + 1);
			{
				if (IS_IN_SEARCH(mb, (total_read + offset)))
				{
					temp_val = *((T*)&tempbuf[offset]);
					prev_val = *((T*)&mb->buffer[total_read + offset]);
					if (temp_val == prev_val)
					{
						++nMatches;
					}
					else
					{
						REMOVE_FROM_SEARCH(mb, (total_read + offset));
					}

				}
			}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_read -= (sizeof(T));//will move sizeof(T) more bytes every read than necessary, but must be here to avoid having bogus value for the last bytes of the mem block
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
		for (int uoffset = 1; uoffset < sizeof(T); ++uoffset) // bug if scan for 1 byte will leave out last byte of memory block
			REMOVE_FROM_SEARCH(mb, (total_read + uoffset));
		mb->size = total_read + sizeof(T);
		mb->matches = nMatches;
		cSearchWindint->pScanOptions->TotalBytesRead += (total_read + sizeof(T));
	}
}
template <typename T>
inline void update_memblockIncreased(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	//CPPOUT << "val : " << val1 << " size : " << mb->data_size << std::endl;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
	int bytes_left; //leave these as signed ints
	int total_read;
	int bytes_to_read;
	int64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		//CPPOUT << "Start address scan this block : "<< hex << mb->addr << std::endl;
		while ((bytes_left > sizeof(T)))
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, (uint64_t*)&bytes_read);

			if (bytes_read != bytes_to_read) //This block is only entered if page is guarded (or gets unallocated) in which case mb->size = sizeof(T) which yields a false positive on first address of block
			{
				//gotta treat this for potential deallocation
				//CPPOUT << "address read " <<  std::hex << reinterpret_cast<int>( mb->addr + total_read) << std::endl;
				break;
			}
			unsigned int offset;
			for (offset = 0; offset + sizeof(T) - 1 < bytes_read; offset += nOffsetUnit)//; offset < (bytes_read - sizeof(T) + 1);
			{
					if (IS_IN_SEARCH(mb, (total_read + offset)))
					{
						temp_val = *((T*)&tempbuf[offset]);
						prev_val = *((T*)&mb->buffer[total_read + offset]);
						if (temp_val > prev_val)
						{
							++nMatches;
						}
						else
						{
							REMOVE_FROM_SEARCH(mb, (total_read + offset));
						}

					}
				}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_read -= (sizeof(T));//will move sizeof(T) more bytes every read than necessary, but must be here to avoid having bogus value for the last bytes of the mem block
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}

		for (int uoffset = 1; uoffset < sizeof(T); ++uoffset)
			REMOVE_FROM_SEARCH(mb, (total_read + uoffset));
		mb->matches = nMatches;
		cSearchWindint->pScanOptions->TotalBytesRead += (total_read + sizeof(T));
	}
}
template <typename T>
inline void update_memblockDecreased(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	//fout << "update memblockDecreased. val : "<< std::hex << val << " size of val : " << std::dec << sizeof(val) << std::endl;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
	int bytes_left; //leave these as signed ints
	int total_read;
	int bytes_to_read;
	int64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		while ((bytes_left > sizeof(T)))
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, (uint64_t*)&bytes_read);

			if (bytes_read != bytes_to_read) //This block is only entered if page is guarded (or gets unallocated) in which case mb->size = sizeof(T) which yields a false positive on first address of block
			{
				//gotta treat this for potential deallocation
				//CPPOUT << "address read " <<  std::hex << reinterpret_cast<int>( mb->addr + total_read) << std::endl;
				break;
			}
			unsigned int offset;
			for (offset = 0; offset + sizeof(T) - 1 < bytes_read; offset += nOffsetUnit)//; offset < (bytes_read - sizeof(T) + 1);
			{
				if ((IS_IN_SEARCH(mb, (total_read + offset))))//insert (offset + sizeof(T)) < bytes_read) check before reading for safety
				{
					temp_val = *((T*)&tempbuf[offset]);
					prev_val = *((T*)&mb->buffer[total_read + offset]);
					if ((temp_val < prev_val))// && ((offset + sizeof(T)) < bytes_read))
					{
						++nMatches;
					}
					else
					{
						REMOVE_FROM_SEARCH(mb, (total_read + offset));
					}

				}
			}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_read -= (sizeof(T));//will move sizeof(T) more bytes every read than necessary, but must be here to avoid having bogus value for the last bytes of the mem block
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
		for (int uoffset = 1; uoffset < sizeof(T); ++uoffset)
			REMOVE_FROM_SEARCH(mb, (total_read + uoffset));
		mb->matches = nMatches;
		cSearchWindint->pScanOptions->TotalBytesRead += (total_read + sizeof(T));
	}
}
template <typename T>
inline void update_memblockLowerThan(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	CPPOUT << "update memblockLower" << std::endl;
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	//CPPOUT << "val : " << val1 << " size : " << mb->data_size << std::endl;
	unsigned char tempbuf[128 * 1024];//was static ! but then not stack allocated / not in cache ?? 128*1024
	unsigned int bytes_left;
	unsigned int total_read;
	unsigned int bytes_to_read;
	uint64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		while (bytes_left)
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, &bytes_read);
			if (bytes_read != bytes_to_read)
				break;
			unsigned int offset;
			for (offset = 0; offset < bytes_read; offset += nOffsetUnit)
			{
				if (IS_IN_SEARCH(mb, (total_read + offset)))
				{
					BOOL is_match = FALSE;
					temp_val = *((T*)&tempbuf[offset]);
					if (temp_val < val)
					{
						++nMatches;
					}
					else
					{
						REMOVE_FROM_SEARCH(mb, (total_read + offset));
					}

				}
			}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
		mb->size = total_read;
		mb->matches = nMatches;
	}
}
template <typename T>
inline void update_memblockBiggerThan(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	CPPOUT << "update memblockBigger" << std::endl;
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	//CPPOUT << "val : " << val1 << " size : " << mb->data_size << std::endl;
	unsigned char tempbuf[128 * 1024];//was static ! but then not stack allocated / not in cache ?? 128*1024
	unsigned int bytes_left;
	unsigned int total_read;
	unsigned int bytes_to_read;
	uint64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		while (bytes_left)
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, &bytes_read);
			if (bytes_read != bytes_to_read)
				break;
			unsigned int offset;
			for (offset = 0; offset < bytes_read; offset += nOffsetUnit)
			{
				if (IS_IN_SEARCH(mb, (total_read + offset)))
				{
					BOOL is_match = FALSE;
					temp_val = *((T*)&tempbuf[offset]);
					if (temp_val > val)
					{
						++nMatches;
					}
					else
					{
						REMOVE_FROM_SEARCH(mb, (total_read + offset));
					}

				}
			}
			memmove(mb->buffer + total_read, tempbuf, bytes_read); //try memmove for optimisation // was memcpy
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}
		mb->size = total_read;
		mb->matches = nMatches;
	}
}
template <typename T>
inline void update_memblockUnknownVal(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	CPPOUT << "update memblockUnknown" << std::endl;
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	//CPPOUT << "val : " << val1 << " size : " << mb->data_size << std::endl;
	unsigned char tempbuf[128 * 1024];
	unsigned int bytes_left;
	unsigned int total_read;
	unsigned int bytes_to_read;
	uint64_t bytes_read;
	if (mb->matches > 0)
	{
		T temp_val;
		T prev_val;
		bytes_left = mb->size;
		total_read = 0;
		int nMatches = 0;
		while (bytes_left)
		{
			bytes_to_read = (bytes_left > sizeof(tempbuf)) ? sizeof(tempbuf) : bytes_left;
			ReadProcessMemory(mb->hProc, mb->addr + total_read, tempbuf, bytes_to_read, &bytes_read);
			if (bytes_read != bytes_to_read)
				break;
			memset(mb->searchmask + (total_read / 8), 0xff, bytes_read / 8); // all bytes get set to 0xFF
			nMatches += bytes_read;
			memcpy(mb->buffer + total_read, tempbuf, bytes_read);
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}

		mb->size = total_read;
		mb->matches = nMatches;
	}
}
inline MEMBLOCK* create_scan(SearchWindow* cSearchWindint, int data_size)
{
	MEMBLOCK *mb_list = NULL;
	MEMORY_BASIC_INFORMATION meminfo;
	unsigned char *addr = 0;

	HANDLE hProc = DebuggedProc.hwnd;

	if (hProc)
	{
		while (1)
		{
			if (VirtualQueryEx(hProc, addr, &meminfo, sizeof(meminfo)) == 0)
			{
				break;
			}

			//#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

			//if ((meminfo.State & MEM_COMMIT) && (meminfo.Protect & WRITABLE)) original code
			if (cSearchWindint->FilterRegions(meminfo))
			{

				MEMBLOCK *mb = create_memblock(hProc, &meminfo, data_size);
				if (mb)
				{
					//           update_memblock (mb);
					mb->next = mb_list;
					mb_list = mb;
				}
				cSearchWindint->pScanOptions->TotalBytesRead += meminfo.RegionSize;
			}
			addr = (unsigned char*)meminfo.BaseAddress + meminfo.RegionSize;
		}
	}

	return mb_list;
}

void free_scan(MEMBLOCK *mb_list)
{
	while (mb_list)
	{
		MEMBLOCK *mb = mb_list;
		mb_list = mb_list->next;
		free_memblock(mb);
	}
}

template <typename T>
void update_scan(SearchWindow* cSearchWindint, MEMBLOCK *mb_list, SEARCH_CONDITION condition, T val)
{
	MEMBLOCK *mb = mb_list;
	switch (condition)
	{
	case COND_EQUALS :
		if (cSearchWindint->pScanOptions->FirstScan)
		{
			while (mb)
			{
				update_memblockEqual(cSearchWindint, mb, condition, val);
				mb = mb->next;
			}
		}
		else
		{
			while (mb)
			{
				update_memblockEqualNextScan(cSearchWindint, mb, condition, val);
				mb = mb->next;
			}
		}
		break;
	case COND_BIGGERTHAN:
		while (mb)
		{
			update_memblockBiggerThan(cSearchWindint, mb, condition, val);
			mb = mb->next;
		}
		break;
	case COND_LOWERTHAN:
		while (mb)
		{
			update_memblockLowerThan(cSearchWindint, mb, condition, val);
			mb = mb->next;
		}
		break;
	case COND_UNCONDITIONAL: // will turn all matches on
		if (cSearchWindint->pScanOptions->FirstScan)
		{
			while (mb)
			{
				update_memblockUnknownVal(cSearchWindint, mb, condition, val);
				mb = mb->next;
			}
			break;
		}
		else
		{
			;//leave as it is, else using unknown after first scan will reset all results (BAD)
		}
	case COND_DECREASED:
		while (mb)
		{
			update_memblockDecreased(cSearchWindint, mb, condition, val);
			mb = mb->next;
		}
		break;
	case COND_INCREASED:
		{
			while (mb)
			{
				update_memblockIncreased(cSearchWindint, mb, condition, val);
				mb = mb->next;
			}
		}
		break;
	case COND_CHANGED:
	{
		LOUT << "Changed scan" << endl;
		while (mb)
		{
			update_memblockChanged(cSearchWindint, mb, condition, val);
			mb = mb->next;
		}
	}
	break;
	case COND_UNCHANGED:
	{
		LOUT << "Unchanged scan" << endl;
		while (mb)
		{
			update_memblockUnChanged(cSearchWindint, mb, condition, val);
			mb = mb->next;
		}
	}
	break;
	}
	CPPOUT << "end of update scan" << std::endl;
}
void dump_scan_info(MEMBLOCK *mb_list)
{
	MEMBLOCK *mb = mb_list;

	while (mb)
	{
		int i;
		printf("0x%08x %d\r\n", mb->addr, mb->size);

		for (i = 0; i < mb->size; i++)
		{
			printf("%02x", mb->buffer[i]);
		}
		printf("\r\n");

		mb = mb->next;
	}
}

void poke(HANDLE hProc, int data_size, unsigned int addr, unsigned int val)
{
	if (WriteProcessMemory(hProc, (void*)addr, &val, data_size, NULL) == 0)
	{
		printf("poke failed\r\n");
	}
}

unsigned int peek(HANDLE hProc, int data_size, unsigned int addr)
{
	unsigned int val = 0;

	if (ReadProcessMemory(hProc, (void*)addr, &val, data_size, NULL) == 0)
	{
		LOUT << "poke failed " << endl;
	}

	return val;
}

void print_matches(MEMBLOCK *mb_list, Ui_DialogResults* pResultWindow, SearchWindow* pSearchWindow)
{
	unsigned int offset;
	MEMBLOCK *mb = mb_list;
	std::string str;
	pResultWindow->_vecResultsAddr.clear(); // clears the results c
	CPPOUT << "result vector size " << pResultWindow->_vecResultsAddr.size() << std::endl;
	while (mb)
	{
		for (offset = 0; offset < mb->size; offset += pSearchWindow->pScanOptions->ScanOffset)//mb->data_size)
		{
			if (IS_IN_SEARCH(mb, offset))
			{
				uint64_t val = peek(mb->hProc, mb->data_size, (unsigned int)mb->addr + offset);
				LOUT << "val is : " << val << endl;
				str = pSearchWindow->ModMap->FetchModuleName(reinterpret_cast<int64_t>(mb->addr + offset));
				if (str != "unknown")
				{
					QTreeWidgetItem * itm = new QTreeWidgetItem(pResultWindow->treeWidget);
					itm->setText(0, ReturnStrFromHexaInt((int64_t)mb->addr + offset).c_str());
					itm->setTextColor(0, Qt::darkGreen);
					//if (pSearchWindow->ui.cbHex->isChecked()) //doesnt work
					//	itm->setText(1, ReturnStrFromHexaInt(val).c_str());
					//else
						itm->setText(1, ReturnStrFromDecInt(val).c_str());
				}
				else
				{
					QTreeWidgetItem * itm = new QTreeWidgetItem(pResultWindow->treeWidget);
					itm->setText(0, ReturnStrFromHexaInt((int64_t)mb->addr + offset).c_str());
					if (pSearchWindow->ui.cbHex->isChecked())
					{
						itm->setText(1, ReturnStrFromHexaInt(val).c_str());
						LOUT << "hex " << ReturnStrFromDecInt(val) << endl;
					}
					else
					{
						itm->setText(1, ReturnStrFromDecInt(val).c_str());
						LOUT << "dec " << ReturnStrFromDecInt(val) << endl;
					}
				}
				pResultWindow->_vecResultsAddr.push_back((int64_t)mb->addr + offset);
			}
		}

		mb = mb->next;
	}
	CPPOUT << "result vector size " << pResultWindow->_vecResultsAddr.size() << std::endl;
}
void OutputResultHardDisk(MEMBLOCK *mb_list, Ui_DialogResults* pResultWindow, SearchWindow* pSearchWindow, std::vector <uint64_t> &addressVec)
{
	unsigned int offset;
	MEMBLOCK *mb = mb_list;
	std::string str;
	addressVec.clear(); // clears the results c
	CPPOUT << "result vector size " << addressVec.size() << std::endl;
	while (mb)
	{
		for (offset = 0; offset < mb->size; offset += pSearchWindow->pScanOptions->ScanOffset)//mb->data_size)
		{
			if (IS_IN_SEARCH(mb, offset))
			{
				uint64_t val = peek(mb->hProc, mb->data_size, (unsigned int)mb->addr + offset);
				addressVec.push_back((int64_t)mb->addr + offset);
			}
		}

		mb = mb->next;
	}
	string CEfilename = "CPPADDRESSES.FIRST";
	ofstream file;
	file.open(CEfilename, ios::binary);
	if (!file.is_open())
		CPPOUT << "error opening file." << endl;
	else
		CPPOUT << "file open." << endl;
	//
	int uuu = 0;
	int o = 0x11111111;
	file.write((char*)&o, sizeof(uint32_t));
	file.write((char*)&o, 3);
	cout << "int " << hex << o << " end" << endl;
	o = addressVec[0];
	for (int jjj = 0; jjj < addressVec.size() ; ++jjj)
	{
		o = addressVec[jjj];
		file.write((char*)&o, sizeof(uint64_t));
	}
	CPPOUT << "result vector size " << addressVec.size() << std::endl;
}

int get_match_count(MEMBLOCK *mb_list)
{
	MEMBLOCK *mb = mb_list;
	int count = 0;

	while (mb)
	{
		count += mb->matches;
		mb = mb->next;
	}

	return count;
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
bool CreateThreadScans(MEMBLOCK *&mb, MEMBLOCK *&mb2, MEMBLOCK *&mbsave2, uint64_t bytesPerThread)
{
	uint64_t currentThreadSize = 0;
	if (!mb)
	{
		CPPOUT << "this thread will not be launched" << std::endl;
		return false;
	}
	while (mb)
	{
		currentThreadSize += mb->size;
		if (currentThreadSize >= bytesPerThread)
		{
			mb2 = mb->next;
			mb->next = nullptr;
			break;
		}
		else
		{
			mb = mb->next;
			if (!mb)
			{
				CPPOUT << "this thread will not be launched" << std::endl;
					return false;
			}
		}
	}
	mbsave2 = mb2;
	CPPOUT << "this thread will read that many bytes : " << currentThreadSize << std::endl;
	return true;
}
template <typename T>
void(*memfunc)(SearchWindow* cSearchWindint, MEMBLOCK *mb_list, SEARCH_CONDITION condition, T val);
template <typename T>
MEMBLOCK* ui_new_scan(SearchWindow* cSearchWindint, uint32_t data_size, T start_val, SEARCH_CONDITION start_cond)
{
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
		StartCounter(CounterStart, PCFreq);
	MEMBLOCK *scan = NULL;
	while (1)
	{
		scan = create_scan(cSearchWindint, data_size);
		if (scan) 
			break;
		QMessageBox::warning(cSearchWindint, "Error", "Invalid Scan", QMessageBox::Ok);
		break; //else a dead process handle will keeps iterating over this error
	}
	CPPOUT << "creating scan took : " << GetCounter(CounterStart, PCFreq) << std::endl;
	bool usethreads = 1;
	if (usethreads)
	{
		int nNumberOfCores = std::thread::hardware_concurrency();
		uint64_t bytesPerThread = (cSearchWindint->pScanOptions->TotalBytesRead) / nNumberOfCores;
		fout << "You have : " << dec << nNumberOfCores << " cores.each thread will scan for " << std::hex << bytesPerThread << " bytes." << std::endl;
		std::vector <MEMBLOCK *> mbVec(nNumberOfCores);
		std::vector <MEMBLOCK *> mbSaveVec(nNumberOfCores);
		std::vector <std::thread *> ThreadsVec(nNumberOfCores);
		mbVec[0] = scan;
		mbSaveVec[0] = scan;
		memfunc<T> = update_scan;
		uint32_t nNumberOfThreads = 0;
		for (int iii = 0 ; iii < nNumberOfCores; ++iii)
		{
			if (iii < nNumberOfCores -1) // iii < 7 on most modern computers
			{
				if (CreateThreadScans(mbVec[iii], mbVec[iii + 1], mbSaveVec[iii + 1], bytesPerThread))
				{
					ThreadsVec[iii] = new std::thread(memfunc<T>, cSearchWindint, mbSaveVec[iii], start_cond, start_val);//fire thread
					++nNumberOfThreads;
				}
				else
				{
					ThreadsVec[iii] = new std::thread(memfunc<T>, cSearchWindint, mbSaveVec[iii], start_cond, start_val);//fire thread
					++nNumberOfThreads;
					break;
				}
			}
			else
			{
				// Last thread (supposedly) being launched, CreateThreadScans should always return 0.
					ThreadsVec[iii] = new std::thread(memfunc<T>, cSearchWindint, mbSaveVec[iii], start_cond, start_val);//fire thread
					++nNumberOfThreads;
					break;
			}
		}
		for (int iii = 0; iii < nNumberOfThreads; ++iii)
		{
			ThreadsVec[iii]->join();
			delete ThreadsVec[iii];
			if (iii < (nNumberOfThreads-1))
			{
				mbVec[iii]->next = mbSaveVec[iii + 1];
			}
		}
		CPPOUT << "total threads : " << nNumberOfThreads << std::endl;
	}
	else
	update_scan(cSearchWindint, scan, start_cond, start_val);
	return scan;
}

void ui_poke(HANDLE hProc, int data_size)
{
	unsigned int addr;
	unsigned int val;
	char s[20];

	printf("Enter the address: ");
	fgets(s, sizeof(s), stdin);
	addr = str2int(s);

	printf("\r\nEnter the value: ");
	fgets(s, sizeof(s), stdin);
	val = str2int(s);
	printf("\r\n");

	poke(hProc, data_size, addr, val);
}

template <typename T>
int SearchWindow::ui_run_scan(MEMBLOCK *scan1, uint32_t data_size, T start_val, SEARCH_CONDITION start_cond, SCAN_CONDITION scan_condition)
{
	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	_mbScan = DebuggedProc.mb;
	switch (scan_condition)
	{
	case NEW_SCAN:
		PCFreq = 0.0;
		CounterStart = 0;
		StartCounter(CounterStart, PCFreq);
		free_scan(_mbScan);
		DebuggedProc.mb = ui_new_scan(this, data_size, start_val, start_cond);
		CPPOUT << "Creating scan and updating it took : " << GetCounter(CounterStart, PCFreq) << "total bytes scanned " << pScanOptions->TotalBytesRead << " /8 : " << ((pScanOptions->TotalBytesRead) + 1) / 8 << std::endl;
		fout << "NEW_SCAN, matches found " << get_match_count(DebuggedProc.mb) << " start condition : " << scan_condition << " start val : " << std::dec << start_val << " data size : " << data_size << std::endl;
		break;
	case NEXT_SCAN:
		update_scan(this, _mbScan, start_cond, start_val);
		break;
	}
	return get_match_count(DebuggedProc.mb);
}
uint32_t SearchWindow::ReturnDataSize()
{
	switch (static_cast<DATA_SIZE>(ui.comboBValueType->currentIndex()))
	{
	case DATA_SIZE::BYTE_:
		return 1;
	case DATA_SIZE::TWOBYTES_:
		return 2;
	case DATA_SIZE::INT32_:
		return sizeof(int);
	case DATA_SIZE::INT64_:
		return 8;
	case DATA_SIZE::FLOAT_:
		return  sizeof(float);
	case DATA_SIZE::DOUBLE_:
		return sizeof(double);
	case DATA_SIZE::STRING_:
		return 0;
	}
}
template <typename T>
void CompareFn(T& lhs, T& rhs, int compare)
{
	bool result = false;
	switch (compare)
	{
	case 1:
		result = lhs > rhs;
		break;
	case 2:
		result = lhs < rhs;
		break;
	}
}
void ScanParameterBase::UpdateSelectedScanOptions(SearchWindow * pSearchWindow, bool firstScan) {
	this->CurrentScanFastScan = pSearchWindow->ui.cbFastScan->isChecked();
	this->GlobalScanType = static_cast <SEARCH_CONDITION> (pSearchWindow->ui.comboBScanType->currentIndex());//Better performance matching ints than comparing strings
	this->GlobalScanValueType = static_cast<DATA_SIZE> (pSearchWindow->ui.comboBValueType->currentIndex());
	this->CurrentScanHexValues = pSearchWindow->ui.cbHex->isChecked();
	this->FirstScan = firstScan;
	this->ValueSize = pSearchWindow->ReturnDataSize();
	this->AcceptedMemoryState;//unfinished
	if (this->CurrentScanFastScan)
		this->ScanOffset = this->ValueSize;
	else
		this->ScanOffset = 1;

	CPPOUT << "scan type " << this->GlobalScanType << std::endl;
}
//defined here for compilation reasons
void ScanParameterBase::GetValue(SearchWindow * pSearchWindow, SCAN_CONDITION NewOrNext)
{
	float nFloat;
	double dDouble;
	QString TextValue = pSearchWindow->ui.LineScanValue->text();
	switch (this->GlobalScanValueType)
	{
	case 1:
		if (this->CurrentScanHexValues)
			nValue8 = TextValue.toInt(0, 16);
		else
			nValue8 = TextValue.toInt(0, 10);
		pSearchWindow->_nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue8, this->GlobalScanType, NewOrNext);
		break;
	case 2:
		if (this->CurrentScanHexValues)
			nValue16 = TextValue.toInt(0, 16);
		else
			nValue16 = TextValue.toInt(0, 10);
		pSearchWindow->_nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue16, this->GlobalScanType, NewOrNext);
		break;
	case 3:
		if (this->CurrentScanHexValues)
			nValue32 = TextValue.toInt(0, 16);
		else
			nValue32 = TextValue.toInt(0, 10);
		pSearchWindow->_nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue32, this->GlobalScanType, NewOrNext);
		break;
	case 4:
		if (this->CurrentScanHexValues)
			nValue64 = TextValue.toLongLong(0, 16);
		else
			nValue64 = TextValue.toLongLong(0, 10);
		pSearchWindow->_nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue64, this->GlobalScanType, NewOrNext);
		break;
	case 5://float case, no hex float
		LOUT << "test" << endl;
			nFloat = TextValue.toFloat();
			LOUT << "value as hex 0x" >> nFloat << endl;
			nValue32 = *(reinterpret_cast<int*>(&nFloat));
			pSearchWindow->_nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, nFloat, this->GlobalScanType, NewOrNext);
		break;
	case 6:
		dDouble = TextValue.toDouble();
		LOUT << "value as hex 0x" >> dDouble << endl;
		nValue64 = *(reinterpret_cast<int64_t*>(&dDouble));
		pSearchWindow->_nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, dDouble, this->GlobalScanType, NewOrNext);
		break;
	default:
		QMessageBox::warning(pSearchWindow, "Error", "This value type is not supported yet", QMessageBox::Ok);
		break;
	}
}
void SearchWindow::someSlot() {
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "Test", "Quit?",
		QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		//QApplication::quit();
	}
	else {

	}
}
void SearchWindow::ShowResults(uint64_t nResults)
{
	_hResult->label_2->setText(ReturnStrFromDecInt(nResults).c_str());
}
bool SearchWindow::FilterRegions(MEMORY_BASIC_INFORMATION &meminfo)
{
	int32_t nAcceptedProtects = ALL_MEM_PROTECTS;
	int32_t nRequiredProtectsWrite = ALL_MEM_PROTECTS;
	int32_t nRequiredProtectsExecute = ALL_MEM_PROTECTS;
	int32_t nRequiredProtectsCoW = ALL_MEM_PROTECTS; // implement this later
	int32_t nBannedProtects = PAGE_GUARD;
	switch (ui.cbWritable->checkState())
	{
	case Qt::PartiallyChecked:
		break;
	case Qt::Checked:
		nRequiredProtectsWrite = 0;
		nRequiredProtectsWrite = (WRITABLE_EXECUTE); // we add required flags accordingly and non matching zones will then be excluded
		break;
	case Qt::Unchecked:
		nAcceptedProtects |= (WRITABLE_EXECUTE); //useless but better be safe than sorry
		nAcceptedProtects ^= (WRITABLE_EXECUTE); // all mem protect flags are up at the start, we remove them accordingly with unchecked boxes
		break;
	}

	if (ui.cbExecutable->checkState() == Qt::PartiallyChecked)
		;
	else if (ui.cbExecutable->checkState() == Qt::Checked)
	{
		nRequiredProtectsExecute = 0;
		nRequiredProtectsExecute = (EXECUTE_WRITABLE);
	}
	else if (ui.cbExecutable->checkState() == Qt::Unchecked)
	{
		nAcceptedProtects |= EXECUTE_WRITABLE; // we don't know what we kicked off so we make sure they are all present before xoring
		nAcceptedProtects ^= EXECUTE_WRITABLE;
	}

	switch (ui.cbCopyOnWrite->checkState())
	{
	case Qt::PartiallyChecked:
		break;
	case Qt::Checked:
		nRequiredProtectsCoW = 0;
		nRequiredProtectsCoW = (COPY_WRITE); // we add required flags accordingly and non matching zones will then be excluded
		break;
	case Qt::Unchecked:
		nAcceptedProtects |= (COPY_WRITE); //useless but better be safe than sorry
		nAcceptedProtects ^= (COPY_WRITE); // all mem protect flags are up at the start, we remove them accordingly with unchecked boxes
		break;
	}

	if (nBannedProtects & meminfo.Protect) // if PAGE_GUARD flag is set, we won't map this section
		return false;
	if ((meminfo.State & MEM_COMMIT) && (meminfo.Protect & nAcceptedProtects))
		if ((meminfo.Protect & nRequiredProtectsExecute) && (meminfo.Protect & nRequiredProtectsWrite) && (meminfo.Protect & nRequiredProtectsCoW))
			return true;

	return false;
}
void SearchWindow::closeEvent(QCloseEvent *event)
{
	//saves scan settings here
	if (_hResult != nullptr)
	{
		_hResult->_Dialog->close();
		_hResult = nullptr;
	}
}
void SearchWindow::IsSetOnTop() {
	if (ui.cbAlwaysOnTop->checkState())
	{
		this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
		this->show();
	}
	else
	{
		this->setWindowFlags(this->windowFlags() ^ Qt::WindowStaysOnTopHint);
		this->show();
	}
}
void SearchWindow::FirstScan()
{
	double PCFreq1 = 0.0;
	__int64 CounterStart2 = 0;
	StartCounter(CounterStart2, PCFreq1);
	if (bInScan == 0)
	{
		bInScan = 1;
		ui.comboBValueType->setDisabled(true);
		ui.cbExecutable->setDisabled(true);
		ui.cbWritable->setDisabled(true);
		ui.cbCopyOnWrite->setDisabled(true);
		ui.lineRangeEnd->setDisabled(true);
		ui.lineRangeStart->setDisabled(true);
		ui.pbFirstNewScan->setText("New Scan");
		pScanOptions->UpdateSelectedScanOptions(this, 1); //1 = new scan
		if (pScanOptions->GlobalScanType != COND_UNCONDITIONAL)
		{
			CPPOUT << "asd " << pScanOptions->GlobalScanType << std::endl;
			QString text = ui.LineScanValue->text();
			if (text == "")//This check should only happen when we try to scan
			{
				QMessageBox::warning(this, "Error", "Enter a value", QMessageBox::Ok); //a value must be filled in for the scan to go through
				return;
			}
			std::string strString = text.toStdString();
			if (!std::all_of(strString.begin(), strString.end(), ::isdigit))
				if (!ui.cbHex->isChecked())//asserts we have no hex number when scanning with hex unchecked
				{//better convert base 16 first and if the function fails then call the input incorrect
					QMessageBox::warning(this, "Invalid number", "This isn't a decimal number.", QMessageBox::Ok);
					return;
				}
		}
		this->ModMap = new Mods;
		*ModMap = UpdateModulesList();
		pScanOptions->GetValue(this, NEW_SCAN);
		//different calls according to value size
		mResultsVec.lock();
		if (_nResults < NUMBER_DISPLAYED_RESULTS)
			print_matches(DebuggedProc.mb, _hResult, this);
		LOUT << "Scan performed. " << _nResults << " results." << std::endl;
#ifdef DEBUG
		std::vector <uint64_t> _vec;
		OutputResultHardDisk(DebuggedProc.mb, _hResult, this, _vec);
#endif
		mResultsVec.unlock();
		ShowResults(_nResults);
		if (ui.comboBScanType->currentIndex() == 3)
			ui.comboBScanType->setCurrentIndex(0);
		ui.pbNextScan->setDisabled(false);
		ui.comboBScanType->clear();
		ui.comboBScanType->addItems(QStringList() << "Exact Value" << "Bigger than.." << "Smaller than.." << "Unknown Value" << "Increased value" << "Decreased value" << "Changed value" << "Unchanged Value");
	}
	else
	{
		bInScan = 0;
		_nResults = 0;
		ui.comboBValueType->setDisabled(false);
		ui.cbExecutable->setDisabled(false);
		ui.cbWritable->setDisabled(false);
		ui.cbCopyOnWrite->setDisabled(false);
		ui.lineRangeEnd->setDisabled(false);
		ui.lineRangeStart->setDisabled(false);
		ui.pbFirstNewScan->setText("First Scan");
		ui.pbNextScan->setDisabled(true);
		mResultsVec.lock();
		_hResult->treeWidget->clear();
		_hResult->_vecResultsAddr.clear(); // so that the other thread will notice and continue;
		mResultsVec.unlock();
		ShowResults(_nResults);
		delete ModMap;
		this->ModMap = nullptr;
		pScanOptions->TotalBytesRead = 0;
		ui.comboBScanType->clear();
		ui.comboBScanType->addItems(QStringList() << "Exact Value" << "Bigger than.." << "Smaller than.." << "Unknown initial value");
	}
	CPPOUT << "Whole first scan process took : " << GetCounter(CounterStart2, PCFreq1) << std::endl;
	LOUT << "Whole first scan process took : " << GetCounter(CounterStart2, PCFreq1) << std::endl;
}
void SearchWindow::NextScan()
{
	pScanOptions->UpdateSelectedScanOptions(this, 0); //0 = next scan
	if (pScanOptions->GlobalScanType == COND_EQUALS)
	{
		QString text = ui.LineScanValue->text();
		if (text == "")//This check should only happen when we try to scan
		{
			QMessageBox::warning(this, "Error", "Enter a value", QMessageBox::Ok); //a value must be filled in for the scan to go through
			return;
		}
		std::string strString = text.toStdString();
		if (!std::all_of(strString.begin(), strString.end(), ::isdigit))
			if (!ui.cbHex->isChecked())//asserts we have no hex number when scanning with hex unchecked
			{
				QMessageBox::warning(this, "Invalid number", "This isn't a decimal number.", QMessageBox::Ok);
			}
	}
	pScanOptions->GetValue(this, NEXT_SCAN);
	mResultsVec.lock();
	_hResult->treeWidget->clear();
	if (_nResults < NUMBER_DISPLAYED_RESULTS)
		print_matches(DebuggedProc.mb, _hResult, this);
	mResultsVec.unlock();
	ShowResults(_nResults);
}
SearchWindow::SearchWindow(QMainWindow* parent)
{
	this->setWindowFlags(this->windowFlags() ^ Qt::WindowContextHelpButtonHint);
	this->setWindowFlags(this->windowFlags() | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
	ui.setupUi(this);
	QObject::connect(ui.cbAlwaysOnTop, &QCheckBox::clicked, this, &SearchWindow::IsSetOnTop);
	QObject::connect(ui.pbFirstNewScan, &QPushButton::clicked, this, &SearchWindow::FirstScan);
	QObject::connect(ui.pbNextScan, &QPushButton::clicked, this, &SearchWindow::NextScan);
	pScanOptions = new ScanParameterBase();
}