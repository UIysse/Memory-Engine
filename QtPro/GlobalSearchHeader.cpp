#include"GlobalSearchHeader.h"
#include "InputComment.h"
#include "TypeBox.h"

GlobalSearchInstance::GlobalSearchInstance(QMainWindow *parent, HoldPtr *pHoldPtr)
{
	_pQMainWindow = parent;
	_HoldPtr = pHoldPtr;
	_pResultsWindow = new ResultsWindow(parent, pHoldPtr);
	_pResultsWindow->setAttribute(Qt::WA_DeleteOnClose);
	_pResultsWindow->show();
	_pSearchWindow = new SearchWindow(parent);
	_pSearchWindow->setAttribute(Qt::WA_DeleteOnClose);
	_pSearchWindow->show();
	_pSearchWindow->_hResult = &_pResultsWindow->ui;
	_pResultsWindow->pSearchWindow = &_pSearchWindow->ui;
	_pSearchWindow->_hResult->_pScanParameterBase = _pSearchWindow->pScanOptions;
	this->ConnectSlots(); // After sub windows are constructed
	//ini threads
	_pResultsWindow->_ThreadStayAlive = 1; // Better keep watching results's thread bool
	this->_th = new std::thread(&GlobalSearchInstance::UpdateResultsValue, this);
	this->_th2 = new std::thread(&GlobalSearchInstance::UpdateSavedValue, this);
}
void GlobalSearchInstance::UpdateResultsValue()
{
	bool bValidMemory(0);
	mResultsVec.lock();
	while (_pResultsWindow->_ThreadStayAlive)
	{
		if (_pResultsWindow->ui._vecResultsAddr.empty())
		{
			mResultsVec.unlock();
			continue;
		}
		uint64_t sizevalue = 0;
		for (int i = 0; i != _pResultsWindow->ui._vecResultsAddr.size(); ++i)
		{
			QTreeWidgetItem *itm = _pResultsWindow->ui.treeWidget->topLevelItem(i);
			bValidMemory = ReadProcessMemory(DebuggedProc.hwnd, (LPCVOID)_pResultsWindow->ui._vecResultsAddr[i], &sizevalue, 4, NULL);
			this->SetValues(itm, sizevalue, 1, bValidMemory);
		}
		mResultsVec.unlock();
		Sleep(200);
	}
}
void GlobalSearchInstance::SetValues(QTreeWidgetItem * itm, unsigned long long nValue, int column, bool bValidMemory)//for some reason connect wont work if we use uint64_t here -> using ulong long
{
	if (bValidMemory == 0)
	{
		itm->setText(column, "???");
		return;
	}
	unsigned long long localnValue = _pSearchWindow->pScanOptions->nValue32; // make it a template variable
	if (localnValue == nValue)
		return; //changed values will now appear as red, nothing is changed otherwise //doesn't work because nValue is always 0
	if (_pSearchWindow->pScanOptions->CurrentScanHexValues)
		itm->setText(column, ReturnStrFromHexaInt(nValue).c_str());
	else
		itm->setText(column, ReturnStrFromDecInt(nValue).c_str());
	if (column == 1)
		itm->setTextColor(column, Qt::red);
}
void GlobalSearchInstance::UpdateSavedValue()
{
	bool bValidMemory(0);
	while (_pResultsWindow->_ThreadStayAlive)
	{
		Sleep(200);
		mSavedVec.lock();
		if (_pResultsWindow->ui._vecSavedAddr.empty())
		{
			mSavedVec.unlock();
			continue;
		}
		uint64_t sizevalue = 0;
		for (int i = 0; i != _pResultsWindow->ui._vecSavedAddr.size(); ++i)
		{
			QTreeWidgetItem *itm = _pResultsWindow->ui.treeWidget_2->topLevelItem(i);
			bValidMemory = ReadProcessMemory(DebuggedProc.hwnd, (LPCVOID)_pResultsWindow->ui._vecSavedAddr[i], &sizevalue, 4, NULL);
			emit _pResultsWindow->UpdateResultsContent(itm, sizevalue, 2, bValidMemory);
		}
		mSavedVec.unlock();
	}
}
void GlobalSearchInstance::AddVariable(QTreeWidgetItem * itm, int column)
{
	mSavedVec.lock();
	_pResultsWindow->ui.itm = new QTreeWidgetItem(_pResultsWindow->ui.treeWidget_2); //could be QTreeWidgetItem *itm
	_pResultsWindow->ui.itm->setText(1, itm->text(0));
	_pResultsWindow->ui.itm->setText(2, itm->text(1));
	_pResultsWindow->ui.itm->setText(3, _pSearchWindow->ui.comboBValueType->currentText());
	_pResultsWindow->ui._vecSavedAddr.push_back(itm->text(0).toULongLong(0, 16));
	mSavedVec.unlock();
}
void GlobalSearchInstance::AddComment(QTreeWidgetItem * itm, int column)
{
	InputComment * pInputComment;
	switch (column)
	{
	case 0:
		pInputComment = new InputComment(_pResultsWindow, _pResultsWindow, itm, column); //change first arg to _pQMainWindow ?
		pInputComment->setWindowFlags(pInputComment->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		pInputComment->setAttribute(Qt::WA_DeleteOnClose);
		pInputComment->show();
		pInputComment->ui.textEdit->setFocus();
		break;
	case 2:
		pInputComment = new InputComment(_pResultsWindow, _pResultsWindow, itm, column);
		pInputComment->setWindowFlags(pInputComment->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		pInputComment->setAttribute(Qt::WA_DeleteOnClose);
		pInputComment->show();
		pInputComment->ui.textEdit->setFocus();
		break;
	case 3:
		InputTypeBox *pTypeBox = new InputTypeBox(_pResultsWindow, _pResultsWindow, itm, column);
		pTypeBox->setWindowFlags(pInputComment->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		pTypeBox->setAttribute(Qt::WA_DeleteOnClose);
		pTypeBox->show();
		break;
	}
}
void GlobalSearchInstance::ConnectSlots() 
{
	QObject::connect(_pResultsWindow->ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(AddVariable(QTreeWidgetItem *, int)));
}
void GlobalSearchInstance::ActivateWindows()
{
	_pResultsWindow->activateWindow();
	_pSearchWindow->activateWindow();
}
GlobalSearchInstance::~GlobalSearchInstance()
{
	_HoldPtr->_bGlobalSearchInstance = false;//redundant
}