#include "qtpro.h"
#include <QtWidgets/QApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("QtProject");
	QCoreApplication::setApplicationName("Application Example");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	QtPro w;
	w.show();
	return a.exec();
}
