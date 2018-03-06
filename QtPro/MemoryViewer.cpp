#include "MemoryViewer.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets>
#include <QMainWindow>
#include <QtWidgets/QMainWindow>
#include <QShortcut>
#include <sstream>
#include "ui_MemoryView.h"
#include "DebuggedProcess.h"
#include "InputGotoBox.h"
#include "ui_ShowDLLs.h"
#include "BeaEngine/BeaEngine.h"
#include "MemRead.h"
/*
Informations sur la structure DISASM de BeaEngine :
http://beatrix2004.free.fr/BeaEngine/structure.php
*/
using namespace std;
MemoryViewer::MemoryViewer(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.viewDlls, &QAction::triggered, this, &MemoryViewer::showDlls);
	QObject::connect(ui.shortcut, &QShortcut::activated, this, &MemoryViewer::ShowGotoDialogBox);
	if (DebuggedProc.hwnd)
		insertDisas(this);
}
void MemoryViewer::showDlls()
{
	ShowDll * showDll = new ShowDll(this);
}
void MemoryViewer::ShowGotoDialogBox()
{
	InputGotoBox * inputgotobox = new InputGotoBox(this, this);
	inputgotobox->setWindowFlags(inputgotobox->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	inputgotobox->setAttribute(Qt::WA_DeleteOnClose);
	inputgotobox->show();
	inputgotobox->ui.textEdit->setFocus();
}
void MemoryViewer::DisassembleNewContent()
{
	//reconstruct disasembling with new address of interest
	ui.treeWidget->clear();
	ui.treeWidget->show();
	if (DebuggedProc.hwnd)
		insertDisas(this);
}
void MemoryViewer::FreeMemoryBlocks(MEMBLOCK *mb_list)
{
	while (mb_list)
	{
		MEMBLOCK *mb = mb_list;
		mb_list = mb_list->next;
		FreeOneMemoryBlock(mb);
	}
}
void MemoryViewer::FreeOneMemoryBlock(MEMBLOCK *mb)
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
int MemoryViewer::insertDisas(MemoryViewer * aDialog)
{
	DebuggedProc.IsHandleValid();
	if (!DebuggedProc.addressOfInterest)
		LOUT_ERROR << "error, no EIP used" << endl;
	FreeMemoryBlocks(DebuggedProc.mb);
	DebuggedProc.mb = QueryMemoryAddrress(DebuggedProc.addressOfInterest);
	if (DebuggedProc.mb)
	{
		LOUT_ERROR << "disasembling";
		ostringstream stre;
		string str;
		QTreeWidgetItem* itm;
		QTreeWidgetItem *itmToSetCurrent(nullptr);
		QFont font("Terminal");
		font.setCapitalization(QFont::AllUppercase);
		font.setPointSize(9);
		font.setStretch(145);
		font.setKerning(false);
		/* ============================= Init datas */
		DISASM MyDisasm;
		int nfalse = 0, ntrue = 1;
		int len, i = 0;
		bool Error = nfalse;

		/* ============================= Init the Disasm structure (important !)*/
		(void)memset(&MyDisasm, 0, sizeof(DISASM));

		/* ============================= Init EIP */
		int64_t n;
		n = reinterpret_cast<int64_t>(DebuggedProc.mb->buffer);
		fout << "Address of Interest : " << hex << DebuggedProc.addressOfInterest << endl;
		fout << "Start of memory page : " << hex << DebuggedProc.basePageAddress << endl;
		fout << "size : " << hex << DebuggedProc.mb->size << endl;
		n = n + DebuggedProc.addressOfInterest - DebuggedProc.basePageAddress;
		int64_t nTargetedProcessAddress = DebuggedProc.addressOfInterest;
		cout << "n : " << hex << n << endl;
		MyDisasm.Archi = DebuggedProc.architecture;
		MyDisasm.EIP = reinterpret_cast<int64_t>(DebuggedProc.mb->buffer);//n;// 0x401000;
		MyDisasm.VirtualAddr = DebuggedProc.basePageAddress; //DebuggedProc.addressOfInterest;
		cout << "sizeof " << sizeof(MyDisasm.EIP) << endl;
		cout << "start address : " << hex << MyDisasm.EIP << endl;
		uint64_t nTotalBytesDisasembled = 0;
		QList<QTreeWidgetItem*> items; //Create a list of items that will be assigned to QTreeWidget all at once.
		/* ============================= Loop for Disasm */
		while ((nTotalBytesDisasembled <= DebuggedProc.mb->size)) { // has to be inferior or equal otherwise last x bytes wont be disasembled
			itm = new QTreeWidgetItem;
			itm->setFont(2, font); //font is created at the begining of the function
			itm->setFont(0, font);
			itm->setFont(1, font);
			len = Disasm(&MyDisasm);
			string str1;
			int len2;//because len = -1 for unknown opcodes
			if (len > 0)
				len2 = len;
			else
				len2 = 1;
			nTotalBytesDisasembled += len2;
			str1[len2 * 2] = 0;//assigning null terminator to string
			for (int u = 0; u < len2; ++u)
			{
				if (((((*(unsigned char*)(MyDisasm.EIP + u)) >> 4) & 0xF) < 0xA) && ((((*(unsigned char*)(MyDisasm.EIP + u)) >> 4) & 0xF) >= 0))
					str1[u * 2] = (((*(unsigned char*)(MyDisasm.EIP + u)) >> 4) & 0xF) + '30';
				else
					str1[u * 2] = (((*(unsigned char*)(MyDisasm.EIP + u)) >> 4) & 0xF) + '37';
				if ((((*(unsigned char*)(MyDisasm.EIP + u)) & 0xF) < 0xA) && (((*(unsigned char*)(MyDisasm.EIP + u)) & 0xF) >= 0))
					str1[u * 2 + 1] = ((*(unsigned char*)(MyDisasm.EIP + u)) & 0xF) + '30';
				else
					str1[u * 2 + 1] = ((*(unsigned char*)(MyDisasm.EIP + u)) & 0xF) + '37';
			}
			itm->setText(1, str1.c_str());
			if (len != UNKNOWN_OPCODE) {
				str.clear();
				stre.str("");
				i++;
				stre << std::hex << MyDisasm.VirtualAddr;
				if (MyDisasm.VirtualAddr == (uint64_t)DebuggedProc.addressOfInterest)
					itmToSetCurrent = itm;
				str = stre.str();
				itm->setText(0, str.c_str());
				itm->setText(2, MyDisasm.CompleteInstr);
				//06.03.17 draft color
				string str5(MyDisasm.CompleteInstr);
				if (str5.size() >= 2) // we dereference [1] thus size must be >=2
				{
				    if (str5[1] == 'm')		//(str5.find("jmp") != std::string::npos)   performance gains
				    {
					itm->setBackgroundColor(2, Qt::yellow);
				    }
					else if (str5[0] == 'j') //will be jne jge etc as jmp instructions have been ruled out with first if condition
					{
						itm->setTextColor(2, Qt::red);
						itm->setBackgroundColor(2, Qt::yellow);
					}
					else if (str5[0] == 'c') //will be call instructions
					{
						itm->setBackgroundColor(2, Qt::cyan);
					}
				}
				MyDisasm.EIP += len;
				MyDisasm.VirtualAddr += len;
			}
			else { //error
				i++;
				Error = false;
				++MyDisasm.EIP;
				++MyDisasm.VirtualAddr;
				str.clear();
				stre.str("");
				stre << std::hex << nTargetedProcessAddress;
				str = stre.str();
				itm->setText(0, str.c_str());
				itm->setText(2, "???");
				++nTargetedProcessAddress;
			}
			items << itm;
		}
		ui.treeWidget->addTopLevelItems(items); //Assigning the list of items to the TreeWidget, huge performance gains.
		LOUT << "total disa : " << nTotalBytesDisasembled << " block size " << DebuggedProc.mb->size << endl;
		fout << "finished disasembling." << endl;
		if(itmToSetCurrent)
		aDialog->ui.treeWidget->scrollToItem(itmToSetCurrent, QAbstractItemView::PositionAtTop);
	}
	else
		fout << "Disassembling didn't take place because there is no memory block." << endl;
	return 0;
}
/*
Function used to determine which memory, where it starts, where it ends, to disasemble at once.
It will disasemble a chunck that was not only allcoated at the same time, the chunk will have the same current properties that s current state (commit)
but more importantly current protect.
This is the same way odbg and x64dbg are determining which block to disasemble.
*/

MEMBLOCK * MemoryViewer::QueryMemoryAddrress(int64_t adressQueried)
{
	MEMBLOCK *mb = nullptr;
	uint64_t nStartAdressZero = 0;
	MEMORY_BASIC_INFORMATION meminfo;
	meminfo.BaseAddress = 0;
	meminfo.RegionSize = 0;
	if (DebuggedProc.hwnd)
	{
	while (((uint64_t)meminfo.BaseAddress + meminfo.RegionSize) <= adressQueried)
	{
		if (VirtualQueryEx(DebuggedProc.hwnd, (LPVOID)nStartAdressZero, &meminfo, sizeof(meminfo)) == 0)
			break;
		nStartAdressZero = meminfo.RegionSize + (uint64_t)meminfo.BaseAddress;
	}
#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_EXECUTE | PAGE_EXECUTE_READ)
	if ((meminfo.State & MEM_COMMIT))// && (meminfo.Protect & WRITABLE))
	{
		fout << "Memory pages are writable and commited" << endl;
		mb = create_memblockInsertDisas(DebuggedProc.hwnd, &meminfo, 1);
		fout << "base add : " << hex << meminfo.BaseAddress << endl;
		fout << "wanted add : " << hex << adressQueried << endl;
		fout << "regionsize : " << meminfo.RegionSize << endl;
		adressQueried += meminfo.RegionSize;
	}
	else
	{
		fout << "Memory pages are not writable and/or not MEM comit" << endl;
	}
	}
	else
		fout << "no process handle" << endl;
	DebuggedProc.basePageAddress = (int64_t)meminfo.BaseAddress;
	fillEachMemblock(mb);
	return mb;
}
MEMBLOCK* MemoryViewer::create_memblockInsertDisas(HANDLE hProc, MEMORY_BASIC_INFORMATION *meminfo, int data_size)
{
	MEMBLOCK *mb = new MEMBLOCK; //reinterpret_cast <MEMBLOCK *>(malloc(sizeof(MEMBLOCK)));
	if (mb)
	{
		mb->hProc = hProc;
		mb->addr = reinterpret_cast <unsigned char *>(meminfo->BaseAddress); //For disasembling we disasemble the whole allocated block
		mb->allocationBase = reinterpret_cast <unsigned char *>(meminfo->AllocationBase);
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
MemoryViewer::~MemoryViewer()
{

}