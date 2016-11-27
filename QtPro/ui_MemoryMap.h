/********************************************************************************
** Form generated from reading UI file 'designer139472.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGNER139472_H
#define DESIGNER139472_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogMemoryMap
{
public:
	QTreeWidget *treeWidget;

	void setupUi(QDialog *Dialog)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("DialogMemoryMap"));
		Dialog->setWindowIcon(QIcon("icons/CppEngine.ico"));
		Dialog->setWindowFlags(Dialog->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		Dialog->setFixedSize(1200, 600);
		treeWidget = new QTreeWidget(Dialog);
		QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
		__qtreewidgetitem->setText(0, QStringLiteral("1"));
		treeWidget->setHeaderItem(__qtreewidgetitem);
		treeWidget->setObjectName(QStringLiteral("treeWidget"));
		treeWidget->setGeometry(QRect(0, 0, 1200, 600));
		treeWidget->setHeaderLabels(QStringList() << "Address" << "State" << "Size" << "Owner" << "Section/Info" << "Content" << "Type" << "Access" << "Allocation Access");
		treeWidget->setColumnWidth(0, 80);
		treeWidget->setColumnWidth(1, 150);
		treeWidget->setColumnWidth(2, 80);
		treeWidget->setColumnWidth(3, 110);

		retranslateUi(Dialog);

		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("DialogMemoryMap", "Memory Map", 0));
	} // retranslateUi

};

namespace Ui {
	class DialogMemoryMap : public Ui_DialogMemoryMap {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGNER139472_H
