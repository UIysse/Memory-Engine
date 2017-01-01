/********************************************************************************
** Form generated from reading UI file 'MemoryView.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEMORYVIEW_H
#define UI_MEMORYVIEW_H

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
#include "DebuggedProcess.h"

QT_BEGIN_NAMESPACE
class Ui_Dialog
{
public:
	QTreeWidget *treeWidget;
	QAction * viewDlls;
	QShortcut * shortcut;
	void setupUi(QMainWindow *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Dialog"));
		Dialog->setWindowIcon(QIcon("icons/CppEngine.ico"));
		Dialog->resize(1050, 672);
		treeWidget = new QTreeWidget(Dialog);
		QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
		__qtreewidgetitem->setText(0, QStringLiteral("1"));
		treeWidget->setHeaderItem(__qtreewidgetitem);
		treeWidget->setObjectName(QStringLiteral("treeWidget"));
		treeWidget->setGeometry(QRect(0, 20, 1051, 671));
		QMenu * fileMenu = Dialog->menuBar()->addMenu(("&File"));
		QMenu * searchMenu = Dialog->menuBar()->addMenu("&Search");
		QMenu * viewMenu = Dialog->menuBar()->addMenu("&View");
		viewDlls = viewMenu->addAction("&View DLLs");
		retranslateUi(Dialog);
		shortcut = new QShortcut(QKeySequence("g"), Dialog);
		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QMainWindow *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "Memory Viewer", 0));
	} // retranslateUi

};


class Ui_DialogGotoBox
{
public:
	QDialogButtonBox *buttonBox;
	QPlainTextEdit *plainTextEdit;
	QLineEdit *textEdit;
	QLabel * label;
public:
	void setupUi(QDialog *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Goto Address"));
		Dialog->resize(377, 92);
		buttonBox = new QDialogButtonBox(Dialog);
		buttonBox->setObjectName(QStringLiteral("buttonBox"));
		buttonBox->setGeometry(QRect(290, 20, 81, 241));
		buttonBox->setOrientation(Qt::Vertical);
		buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
		textEdit = new QLineEdit(Dialog);
		textEdit->setObjectName(QStringLiteral("textEdit"));
		textEdit->setGeometry(QRect(20, 40, 241, 31));
		label = new QLabel(Dialog);
		label->setObjectName(QStringLiteral("label"));
		label->setGeometry(QRect(30, 0, 221, 31));

		retranslateUi(Dialog);
		//QObject::connect(buttonBox, &buttonBox->clicked, this, &this->foo);
		//QObject::connect(buttonBox, &QDialogButtonBox::clicked, this, &Ui_DialogGotoBox::foo);
		QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));
		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "Goto Address", 0));
		label->setText(QApplication::translate("Dialog", "Fill in the address you want to go to", 0));
	}

};
	class MemoryView : public Ui_Dialog {};
	class GotoBox : public Ui_DialogGotoBox {};

QT_END_NAMESPACE

#endif // UI_MEMORYVIEW_H
