#ifndef SHOWDLLS_H
#define SHOWDLLS_H
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
#include <QtWidgets/QListView>
class ShowDll;
class ShowDllBase
{
public:
	QPushButton * confirmButton;
	QTreeWidget * treeWidget;
	void setup(QDialog * DllFormClass)
	{
		if (DllFormClass->objectName().isEmpty())
			DllFormClass->setObjectName("ShowDllClass");
		confirmButton = new QPushButton(DllFormClass);
		DllFormClass->setWindowTitle("Loaded Dlls");
		DllFormClass->resize(261, 358);
		treeWidget = new QTreeWidget(DllFormClass);
		treeWidget->setGeometry(QRect(0, 0, 261, 358));
		treeWidget->setHeaderLabel("Modules : ");
		confirmButton->setGeometry(QRect(280, 360, 100, 100));
	}
};
class ShowDll : public QDialog
{
public:
	ShowDll(QMainWindow * parent = 0) : QDialog(parent)
	{
		this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
		ui.setup(this);
		this->show();
	}
private:
	ShowDllBase ui;
};
#endif