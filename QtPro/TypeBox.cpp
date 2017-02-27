#include "TypeBox.h"

InputTypeBox::InputTypeBox(QDialog * parent, ResultsWindow * pResultsWindow, QTreeWidgetItem * itm, int column) : QDialog(parent), _pResultsWindow(pResultsWindow)
{
	ui.setupUi(this, itm, column);
	QPalette Pal(palette());
	this->setAutoFillBackground(true);
	this->setPalette(Pal);
	this->show();
	_itm = itm;
	_column = column;
	QObject::connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &InputTypeBox::confirm);
	QObject::connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &InputTypeBox::decline);
}
void InputTypeBox::confirm()
{
	switch (_column)
	{
	case 3:
		_itm->setText(_column, ui.comboBValueType->currentText());
		break;
	}
}
void InputTypeBox::decline()
{
	this->close();
}
InputTypeBox::~InputTypeBox()
{

}