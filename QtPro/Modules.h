#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_Modules.h"
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;
class Mods
{
public:
	Mods() {}
	~Mods() {}
	std::vector < std::pair<std::pair <uint64_t, uint64_t>, std::string>> vRange;
	std::string FetchModuleName(uint64_t nAddress)
	{
		for (auto it = vRange.cbegin(); it != vRange.cend(); ++it)
		{
			if ((nAddress < it->first.second) && (nAddress >= it->first.first))
				return it->second;
		}
		return "unkown";
	}
	void BuildMap(std::string& szModName,uint64_t nModBaseAddress, uint64_t nModSize)
	{
		std::pair <uint64_t, uint64_t> pa = std::make_pair(nModBaseAddress, nModBaseAddress + nModSize);
		std::pair<std::pair <uint64_t, uint64_t>, std::string> pa2 = std::make_pair(pa, szModName);
		vRange.push_back(pa2);
	}
};
class ModulesWindow : public QDialog
{
	//Q_OBJECT
public:
	ModulesWindow(QMainWindow* parent = 0) //: QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setupUi(this);
		UpdateModulesList(this);
	}
	~ModulesWindow() {}
	//functionalities
	friend Mods UpdateModulesList(ModulesWindow * ModWin = 0);
private:
	Ui::DialogModules ui;
};