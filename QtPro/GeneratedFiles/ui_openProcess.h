/********************************************************************************
** Form generated from reading UI file 'untitled.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UNTITLED_H
#define UI_UNTITLED_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget>
#include "qtpro.h"

QT_BEGIN_NAMESPACE

class Ui_OpenProcess
{
public:
	QTreeWidget *listView;
	QWidget *widget;
	QHBoxLayout *horizontalLayout;
	QPushButton *pushButton_2;
	QPushButton *pushButton;

	void setupUi(QDialog *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Dialog"));
		Dialog->resize(561, 315);
		Dialog->setFixedSize(561, 315);
		listView = new QTreeWidget(Dialog);
		listView->setObjectName(QStringLiteral("listView"));
		listView->setGeometry(QRect(0, 0, 561, 260));
		listView->setHeaderLabels(QStringList() << "Ico" << "PID" << "Process" << "Parent Process" << "Cmd line argument");
		listView->setColumnWidth(0, 30);
		listView->setColumnWidth(1, 60);
		listView->setColumnWidth(2, 250);
		listView->setColumnWidth(3, 150);
		listView->setColumnWidth(4, 150);
		//listView->setStyleSheet("QListWidget::item { border-bottom: 1px solid gray; }");
		widget = new QWidget(Dialog);
		widget->setObjectName(QStringLiteral("widget"));
		widget->setGeometry(QRect(190, 270, 195, 30));
		horizontalLayout = new QHBoxLayout(widget);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		horizontalLayout->setContentsMargins(0, 0, 0, 0);
		pushButton_2 = new QPushButton(widget);
		pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

		horizontalLayout->addWidget(pushButton_2);

		pushButton = new QPushButton(widget);
		pushButton->setObjectName(QStringLiteral("pushButton"));

		horizontalLayout->addWidget(pushButton);


		retranslateUi(Dialog);
		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "Process list", 0));
		pushButton_2->setText(QApplication::translate("Dialog", "Open", 0));
		pushButton->setText(QApplication::translate("Dialog", "Cancel", 0));
	} // retranslateUi

};

namespace Ui {
	class OpenProcess : public Ui_OpenProcess {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UNTITLED_H
