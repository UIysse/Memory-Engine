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
#include "qtpro.h"
#include "ui_MemoryView.h"
#include "DebuggedProcess.h"
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

InputGotoBox::~InputGotoBox()
{

}
void MemoryViewer::UpdateDisassembledContent()
{
	delete ui.treeWidget;
	//reconstruct disasembling with new address of interest
	ui.treeWidget = new QTreeWidget(this);
	QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
	__qtreewidgetitem->setText(0, QStringLiteral("1"));
	ui.treeWidget->setHeaderItem(__qtreewidgetitem);
	ui.treeWidget->setObjectName(QStringLiteral("treeWidget"));
	ui.treeWidget->setGeometry(QRect(0, 20, 1051, 671));
	ui.treeWidget->setHeaderLabels(QStringList() << "Address" << "Bytes" << "Opcode" << "Comment");
	ui.treeWidget->setColumnWidth(0, 110);
	ui.treeWidget->setColumnWidth(1, 150);
	ui.treeWidget->setColumnWidth(2, 250);
	/*
	QPalette p(ui.treeWidget->palette());
	p.setColor(QPalette::Base, Qt::darkGray);
	ui.treeWidget->setPalette(p);
	*/
	ui.treeWidget->show();
	//this->show();
	if (DebuggedProc.hwnd)
		insertDisas(this);
}