#ifndef QTPRO_H
#define QTPRO_H
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDialog>
#include "ui_qtpro.h"
#include "ui_aboutce.h"
#include "ui_openProcess.h"
#include "ui_MemoryView.h"
#include "MemRead.h"
#include <atomic>
#include <fstream>
//#include "ui_ShowDLLs.h
class MemoryViewer; // allows declaration prototype for inserDisas
int insertDisas(MemoryViewer * aDialog);
BOOL GetProcessList(QListWidget * listwidget);
HANDLE  ReturnProcessHandle(QString Qstr);
class QtPro;
class About : public QDialog
{
	Q_OBJECT
public:
	QWidget * QWptr;
	About(QWidget *parent = 0);
	~About();
private:
	Ui::AboutCEClass ui;
};
class QtPro : public QMainWindow
{
	Q_OBJECT

public:
	About* w;
	QtPro(QWidget *parent = 0);
	void closeDialog();
	~QtPro();
	static void OpenHandle();
	private slots:
	void ShowAboutDialog();
	void ShowPickProcess();
	void ShowMemoryView();
	void UpdateProcessName();
public:
	Ui::QtProClass ui;
};

class OpenProcessClass : public QDialog
{
public:
	QtPro * pQtPro;
	OpenProcessClass(QWidget *parent, QtPro *MainWind);
	~OpenProcessClass();
	//friend void QtPro::ShowPickProcess();
	void SelectProcess();
private slots:


public:
	Ui::OpenProcess ui;
};
class MemoryViewer : public QMainWindow
{
	Q_OBJECT
public:
	MemoryViewer(QWidget *parent = 0);
	~MemoryViewer();
	void UpdateDisassembledContent();
	Ui::MemoryView ui;
	public slots:
	void showDlls();
	void ShowGotoDialogBox();
private:
private slots:


};
class InputGotoBox : public QDialog
{
	Q_OBJECT
public:
	MemoryViewer * pMemoryViewer;
	void foo()
	{
		QString text = ui.textEdit->text();
		DebuggedProc.addressOfInterest = text.toULongLong(0, 16);
		pMemoryViewer->UpdateDisassembledContent();
		this->close();
	}
	InputGotoBox::InputGotoBox(QMainWindow * parent, MemoryViewer * pMemoryViewer);
	~InputGotoBox();
	Ui::GotoBox ui;
};
#endif // QTPRO_H
