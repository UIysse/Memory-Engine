#include "InputComment.h"
#include "DebuggedProcess.h"
#include <string>
#include "Debugger.h"
InputComment::InputComment(QDialog * parent, ResultsWindow * pResultsWindow, QTreeWidgetItem * itm, int column) : QDialog(parent), _pResultsWindow(pResultsWindow)
{
	ui.setupUi(this, itm, column);
	QPalette Pal(palette());
	this->setAutoFillBackground(true);
	this->setPalette(Pal);
	this->show();
	_itm = itm;
	_column = column;
	QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &InputComment::confirm);
	QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &InputComment::decline);
}
void InputComment::confirm()
{
	/*
	//goo();
	try
	{
		throw(20);
	}
	catch (int e)
	{
		DRIVOUT << "exception handled" << std::endl;
	}
	*/
	switch (_column)
	{
	case 0:
		_itm->setText(_column, ui.textEdit->text());
		this->close();
		break;
	case 2:
		size_t p = 4;
		QString text = ui.textEdit->text();
		std::string str = text.toStdString();
		for (auto &e : str)
			e = toupper(e);
		uint64_t value;
		 if (str.find('OX') != string::npos)
			value = text.toULongLong(0, 16);
		 else
		 {
			 if (std::all_of(str.begin(), str.end(), ::isdigit))
			 value = text.toULongLong(0, 10);
			 else
			 {
				 QMessageBox::warning(this, "Invalid number", "This isn't a decimal number.", QMessageBox::Ok);
				 break;
			 }
		 }
		uint64_t addr = std::stoull((_itm->text(1)).toStdString(), &p, 16);
		WriteTarget(value, addr, 4);
		_itm->setText(_column, str.c_str());
		this->close();
		break;
	}
}
void InputComment::decline()
{
	this->close();
}
InputComment::~InputComment()
{

}