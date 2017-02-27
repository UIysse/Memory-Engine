
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
#define CPPOUT fout
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
				CPPOUT << "test01 " << std::endl;
				CPPOUT << "address read " <<  std::hex << reinterpret_cast<int>( mb->addr + total_read) << std::endl;
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
				if (bytes_to_read == bytes_left) //this is true when the remaining memory to scan is smaller than the tempbuff array
				{//if this is true we should remove the last few bytes of the block from search
					//for (int uoffset = 1; uoffset < sizeof(T); ++uoffset)
						//REMOVE_FROM_SEARCH(mb, (total_read + bytes_read - sizeof(T) + uoffset));
				}
				else
				{//this is true if there is a larger amount of memory remaining to scan than tempbuff size, in which case we must scan the
				 //tempbuff junctions
				 //if (0)
					//bytes_read -= (sizeof(T)); //could be sizeof(T) -1
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
inline void update_memblockIncreased(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	CPPOUT << "update memblockIncreased" << std::endl;
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
			bytes_left -= bytes_read;
			total_read += bytes_read;
		}

		mb->size = total_read;
		mb->matches = nMatches;
	}
}
template <typename T>
inline void update_memblockDecreased(SearchWindow* cSearchWindint, MEMBLOCK *mb, SEARCH_CONDITION condition, T val)
{
	uint32_t nOffsetUnit = cSearchWindint->pScanOptions->ScanOffset;
	CPPOUT << "update memblockDecreased. val : "<< std::hex << val << " size of val : " << std::dec << sizeof(val) << std::endl;
	unsigned char tempbuf[128 * 1024 + sizeof(T)];//was static ! but then not stack allocated / not in cache ?? 128*1024
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
			for (offset = 0; (offset < (bytes_read - sizeof(T) +1)) ; offset += nOffsetUnit) 
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
			if (bytes_to_read == bytes_left) //this is true when the remaining memory to scan is smaller than the tempbuff array
			{//if this is true we should remove the last few bytes of the block from search
				for (int uoffset = 1; uoffset < sizeof(T) ; ++ uoffset)
				REMOVE_FROM_SEARCH(mb, (total_read + bytes_read - sizeof(T) + uoffset));
			} 
			else 
			{//this is true if there is a larger amount of memory remaining to scan than tempbuff size, in which case we must scan the
				//tempbuff junctions
				//if (0)
					bytes_read -= (sizeof(T)); //could be sizeof(T) -1
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
		while (mb)
		{
			update_memblockEqual(cSearchWindint, mb, condition, val);
			mb = mb->next;
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
			while (mb)
			{
				update_memblockIncreased(cSearchWindint, mb, condition, val);
				mb = mb->next;
			}
		}
	case COND_DECREASED:
		while (mb)
		{
			update_memblockDecreased(cSearchWindint, mb, condition, val);
			mb = mb->next;
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
		printf("poke failed\r\n");
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
				str = pSearchWindow->ModMap->FetchModuleName(reinterpret_cast<int64_t>(mb->addr + offset));
				if (str != "unknown")
				{
					QTreeWidgetItem * itm = new QTreeWidgetItem(pResultWindow->treeWidget);
					itm->setText(0, ReturnStrFromHexaInt((int64_t)mb->addr + offset).c_str());
					itm->setTextColor(0, Qt::darkGreen);
					itm->setText(1, ReturnStrFromHexaInt(val).c_str());
				}
				else
				{
					QTreeWidgetItem * itm = new QTreeWidgetItem(pResultWindow->treeWidget);
					itm->setText(0, ReturnStrFromHexaInt((int64_t)mb->addr + offset).c_str());
					itm->setText(1, ReturnStrFromHexaInt(val).c_str());
				}
				pResultWindow->_vecResultsAddr.push_back((int64_t)mb->addr + offset);
			}
		}

		mb = mb->next;
	}
	CPPOUT << "result vector size " << pResultWindow->_vecResultsAddr.size() << std::endl;
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
		if (scan) break;
		QMessageBox::warning(cSearchWindint, "Error", "Invalid Scan", QMessageBox::Ok);
	}
	CPPOUT << "creating scan took : " << GetCounter(CounterStart, PCFreq) << std::endl;
	bool usethreads = 1;
	if (usethreads)
	{
		int nNumberOfCores = std::thread::hardware_concurrency();
		nNumberOfCores = 8;
		uint64_t bytesPerThread = (cSearchWindint->pScanOptions->TotalBytesRead) / nNumberOfCores;
		CPPOUT << nNumberOfCores << " cores. each thread will scan for " << std::hex << bytesPerThread << " bytes." << std::endl;
		std::vector <MEMBLOCK *> mbVec(nNumberOfCores);
		std::vector <MEMBLOCK *> mbSaveVec(nNumberOfCores);
		std::vector <std::thread *> ThreadsVec(nNumberOfCores);
		mbVec[0] = scan;
		mbSaveVec[0] = scan;
		memfunc<T> = update_scan;
		uint32_t nNumberOfThreads = 0;
		for (int iii = 0 ; iii < nNumberOfCores; ++iii)
		{
			if (iii < 7)
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
	scan = DebuggedProc.mb;
	switch (scan_condition)
	{
	case NEW_SCAN:
		PCFreq = 0.0;
		CounterStart = 0;
		StartCounter(CounterStart, PCFreq);
		free_scan(scan);
		DebuggedProc.mb = ui_new_scan(this, data_size, start_val, start_cond);
		CPPOUT << "Creating scan and updating it took : " << GetCounter(CounterStart, PCFreq) << "total bytes scanned " << pScanOptions->TotalBytesRead << " /8 : " << ((pScanOptions->TotalBytesRead) + 1) / 8 << std::endl;
		CPPOUT << "NEW_SCAN, matches found " << get_match_count(DebuggedProc.mb) << " start condition : " << scan_condition << " start val : " << std::dec << start_val << " data size : " << data_size << std::endl;
		break;
	case NEXT_SCAN:
		update_scan(this, scan, start_cond, start_val);
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
		return 4;
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
		pSearchWindow->nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue8, this->GlobalScanType, NewOrNext);
		break;
	case 2:
		if (this->CurrentScanHexValues)
			nValue16 = TextValue.toInt(0, 16);
		else
			nValue16 = TextValue.toInt(0, 10);
		pSearchWindow->nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue16, this->GlobalScanType, NewOrNext);
		break;
	case 3:
		if (this->CurrentScanHexValues)
			nValue32 = TextValue.toInt(0, 16);
		else
			nValue32 = TextValue.toInt(0, 10);
		pSearchWindow->nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue32, this->GlobalScanType, NewOrNext);
		break;
	case 4:
		if (this->CurrentScanHexValues)
			nValue64 = TextValue.toLongLong(0, 16);
		else
			nValue64 = TextValue.toLongLong(0, 10);
		pSearchWindow->nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue64, this->GlobalScanType, NewOrNext);
		break;
	case 5://float case, no hex float
			nFloat = TextValue.toFloat();
			nValue32 = *(reinterpret_cast<int*>(&nFloat));
			pSearchWindow->nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue32, this->GlobalScanType, NewOrNext);
		break;
	case 6:
		dDouble = TextValue.toDouble();
		nValue64 = *(reinterpret_cast<int64_t*>(&dDouble));
		pSearchWindow->nResults = pSearchWindow->ui_run_scan(DebuggedProc.mb, this->ValueSize, this->nValue64, this->GlobalScanType, NewOrNext);
		break;
	default:
		QMessageBox::warning(pSearchWindow, "Error", "This value type is not supported yet", QMessageBox::Ok);
		break;
	}
}