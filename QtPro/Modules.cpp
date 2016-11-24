#include "Modules.h"
#include "DebuggedProcess.h"
#include <Psapi.h>
#include <string>
#include <sstream>
void ModulesWindow::UpdateModulesList() {
	if (DebuggedProc.IsHandleValid())
	{
		HMODULE hMods[1024];
		DWORD cbNeeded;
		MODULEINFO modinfo;
		unsigned int i;
		if (EnumProcessModulesEx(DebuggedProc.hwnd, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
		{
			for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
			{
				wchar_t szModName[MAX_PATH];

				// Get the full path to the module's file.

				if (GetModuleFileNameEx(DebuggedProc.hwnd, hMods[i], szModName, MAX_PATH))
				{
					GetModuleInformation(DebuggedProc.hwnd, hMods[i], &modinfo, sizeof(modinfo));
					std::wstring ws(szModName);
					// your new String
					std::string str(ws.begin(), ws.end());
					std::string str2(str.begin()+ str.rfind("\\")+1, str.end()); //will remove the path from the string
					QTreeWidgetItem * itm = new QTreeWidgetItem(ui.treeWidget);
					itm->setText(0, str2.c_str());
					//str2 = std::to_string((int64_t)modinfo.lpBaseOfDll, hex);
					std::stringstream ss;
					ss << std::hex << modinfo.lpBaseOfDll << std::endl;
					ss >> str2;
					itm->setText(1, str2.c_str());
					ss << std::hex << modinfo.SizeOfImage << std::endl;
					ss >> str2;
					itm->setText(2, str2.c_str());
					ss << std::hex << modinfo.EntryPoint << std::endl;
					ss >> str2;
					itm->setText(3, str2.c_str());
				}
			}
		}

	}
}