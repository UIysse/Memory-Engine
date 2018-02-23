#include"GlobalSearchHeader.h"

GlobalSearchInstance::GlobalSearchInstance(QMainWindow *parent, HoldPtr *pHoldPtr)
{
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
	//ini threads
	_ThreadStayAlive = 1;
	//this->_th = new std::thread(&ResultsWindow::UpdateResultsValue, this);
	//this->_th2 = new std::thread(&ResultsWindow::UpdateSavedValue, this);
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