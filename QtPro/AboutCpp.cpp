#include "AboutCpp.h"
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include <QCloseEvent>

About::About(QWidget *parent) : QDialog(parent)
{
	this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
	QWptr = parent;
	ui.setupUi(this);
}
About::~About()
{
	//QWptr->closeDialog();
}