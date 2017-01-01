#include "InputGotoBox.h"



InputGotoBox::InputGotoBox(QMainWindow * parent, MemoryViewer * pMemoryVwr) : QDialog(parent), pMemoryViewer(pMemoryVwr)
{
	ui.setupUi(this);
	QPalette Pal(palette());
	// set black background
	Pal.setColor(QPalette::Background, Qt::darkGray);
	this->setAutoFillBackground(true);
	this->setPalette(Pal);
	this->show();
	QObject::connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &InputGotoBox::foo);
}
void InputGotoBox::foo()
{
	QString text = ui.textEdit->text();
	DebuggedProc.addressOfInterest = text.toULongLong(0, 16);
	pMemoryViewer->UpdateDisassembledContent();
	this->close();
}
InputGotoBox::~InputGotoBox()
{

}