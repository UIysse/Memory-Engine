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
#include "ui_MemoryView.h"
#include "DebuggedProcess.h"
#include "InputGotoBox.h"
#include "ui_ShowDLLs.h"
MemoryViewer::MemoryViewer(QWidget *parent) //: QMainWindow(parent)
{
	ui.setupUi(this);

	ui.treeWidget->setHeaderLabels(QStringList() << "Address" << "Bytes" << "Opcode" << "Comment");
	ui.treeWidget->setColumnWidth(0, 110);
	ui.treeWidget->setColumnWidth(1, 150);
	ui.treeWidget->setColumnWidth(2, 250);
	if (DebuggedProc.hwnd)
		insertDisas(this);
	QObject::connect(ui.viewDlls, &QAction::triggered, this, &MemoryViewer::showDlls);
	QObject::connect(ui.shortcut, &QShortcut::activated, this, &MemoryViewer::ShowGotoDialogBox);
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

MemoryViewer::~MemoryViewer()
{

}