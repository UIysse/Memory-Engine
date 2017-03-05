/********************************************************************************
** Form generated from reading UI file 'designer112000.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGNER112000_H
#define DESIGNER112000_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDialog>
#include "DebuggedProcess.h"
#include "MemRead.h"
#include <string>
#include <fstream>
QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
	QPlainTextEdit *plainTextEdit;
	std::string str2;
	void setupUi(QDialog *Form)
	{
		if (Form->objectName().isEmpty())
			Form->setObjectName(QStringLiteral("Logs"));
		Form->setWindowIcon(QIcon("icons/CppEngine.ico"));
		Form->setFixedSize(265, 461);
		plainTextEdit = new QPlainTextEdit(Form);
		plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
		plainTextEdit->setGeometry(QRect(0, 0, 265, 461));
		plainTextEdit->setReadOnly(true);
		retranslateUi(Form);
		QMetaObject::connectSlotsByName(Form);
	} // setupUi

	void retranslateUi(QWidget *Form)
	{
		Form->setWindowTitle(QApplication::translate("Cpp Engine Logs", "Cpp Engine Logs", 0));
	} // retranslateUi

};

namespace Ui {
	class LogsForm : public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGNER112000_H
