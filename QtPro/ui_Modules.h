#/********************************************************************************
** Form generated from reading UI file 'designer124732.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGNER124732_H
#define DESIGNER124732_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogModules
{
public:
	QTreeWidget *treeWidget;

	void setupUi(QDialog *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Modules"));
		Dialog->setWindowIcon(QIcon("icons/CppEngine.ico"));
		Dialog->setFixedSize(689, 321);
		treeWidget = new QTreeWidget(Dialog);
		QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
		__qtreewidgetitem->setText(0, QStringLiteral("1"));
		treeWidget->setHeaderItem(__qtreewidgetitem);
		treeWidget->setObjectName(QStringLiteral("treeWidget"));
		treeWidget->setGeometry(QRect(0, 0, 689, 321));
		treeWidget->setHeaderLabels(QStringList() << "Name" << "Base Address" << "Size" << "Entrypoint");
		treeWidget->setColumnWidth(0, 150);
		treeWidget->setColumnWidth(1, 150);
		treeWidget->setColumnWidth(2, 80);
		treeWidget->setColumnWidth(3, 110);

		retranslateUi(Dialog);

		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Modules", "Modules", 0));
	} // retranslateUi

};

namespace Ui {
	class DialogModules : public Ui_DialogModules {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGNER124732_H
