/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog2
{
public:
	QLabel *label;
	QPushButton *pushButton;

	void setupUi(QDialog *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Dialog"));
		Dialog->resize(400, 300);
		label = new QLabel(Dialog);
		label->setObjectName(QStringLiteral("label"));
		label->setGeometry(QRect(70, 70, 211, 101));
		pushButton = new QPushButton(Dialog);
		pushButton->setObjectName(QStringLiteral("pushButton"));
		pushButton->setGeometry(QRect(170, 230, 93, 28));

		retranslateUi(Dialog);

		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
		label->setText(QApplication::translate("Dialog", "All credits go to DarkByte", 0));
		pushButton->setText(QApplication::translate("Dialog", "Ok", 0));
	} // retranslateUi

};

namespace Ui {
	class Dialog : public Ui_Dialog2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
