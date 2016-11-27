/********************************************************************************
** Form generated from reading UI file 'designer130984.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGNER130984_H
#define DESIGNER130984_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogThreadList
{
public:
	QTreeWidget *treeWidget;

	void setupUi(QDialog *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("ThreadListView"));
		Dialog->setWindowIcon(QIcon("icons/CppEngine.ico"));
		Dialog->setFixedSize(350, 362);
		treeWidget = new QTreeWidget(Dialog);
		QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
		__qtreewidgetitem->setText(0, QStringLiteral("1"));
		treeWidget->setHeaderItem(__qtreewidgetitem);
		treeWidget->setHeaderLabels(QStringList() << "ThreadId" << "Priority" << "Start address");
		treeWidget->setObjectName(QStringLiteral("treeWidget"));
		treeWidget->setGeometry(QRect(0, 0, 350, 362));
		treeWidget->setColumnWidth(0, 70);
		treeWidget->setColumnWidth(1, 100);
		treeWidget->setColumnWidth(2, 180);

		retranslateUi(Dialog);

		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("ThreadListView", "Thread List", 0));
	} // retranslateUi

};

namespace Ui {
	class Ui_ThreadList : public Ui_DialogThreadList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGNER130984_H
