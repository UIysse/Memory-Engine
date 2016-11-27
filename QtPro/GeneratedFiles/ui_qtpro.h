/********************************************************************************
** Form generated from reading UI file 'qtpro.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTPRO_H
#define UI_QTPRO_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtProClass
{
public:
	QAction *action_Exit;
	QAction *actionE_xit;
	QAction *actionCheat_Engine_Help_F1;
	QAction *actionCheat_Engine_Tutorial;
	QAction *actionAbout;
	QAction *actionAdd_Scan_Table;
	QAction *actionClear_list;
	QAction *actionOpen_Process;
	QAction *actionOpen_File;
	QAction *actionSave;
	QAction *actionSaved_as;
	QAction *actionLoad;
	QAction *actionSave_current_scanresults;
	QAction *actionGenerate_generic_trainer_lua_script_from_table;
	QAction *actionQuit;
	QAction *actionSettings;
	QAction *actionShow_Cheat_Table_Lua_Script;
	QAction *actionCreate_Form;
	QAction *actionResynchronyze_forms_with_Lua;
	QAction *actionAdd_file;
	QAction *actionHook_Direct3D;
	QAction *actionSet_custom_CrossHair;
	QAction *actionLock_mouse_in_game_window;
	QAction *actionSnapshot_Handler;
	QWidget *centralWidget;
	QToolButton * toolButton;
	QToolButton * toolButMod;
	QToolButton * toolButLogs;
	QToolButton * toolButSearch;
	QToolButton * toolButMemoryView;
	QToolButton* toolButMemoryMap;
	QToolButton* toolButThreads;
	QMenuBar *menuBar;
	QMenu *menu_File;
	QMenu *menuEdit;
	QMenu *menuTable;
	QMenu *menuD3D;
	QMenu *menuHelp;
	QStatusBar *statusBar;
	QLabel * lblProcessName;

	void setupUi(QMainWindow *QtProClass)
	{
		if (QtProClass->objectName().isEmpty())
			QtProClass->setObjectName(QStringLiteral("QtProClass"));
		QtProClass->resize(1054, 688);
		QtProClass->setWindowIcon(QIcon("icons/QtPro.ico"));
		action_Exit = new QAction(QtProClass);
		action_Exit->setObjectName(QStringLiteral("action_Exit"));
		actionE_xit = new QAction(QtProClass);
		actionE_xit->setObjectName(QStringLiteral("actionE_xit"));
		actionCheat_Engine_Help_F1 = new QAction(QtProClass);
		actionCheat_Engine_Help_F1->setObjectName(QStringLiteral("actionCheat_Engine_Help_F1"));
		actionCheat_Engine_Tutorial = new QAction(QtProClass);
		actionCheat_Engine_Tutorial->setObjectName(QStringLiteral("actionCheat_Engine_Tutorial"));
		actionAbout = new QAction(QtProClass);
		actionAbout->setObjectName(QStringLiteral("actionAbout"));
		actionAdd_Scan_Table = new QAction(QtProClass);
		actionAdd_Scan_Table->setObjectName(QStringLiteral("actionAdd_Scan_Table"));
		actionClear_list = new QAction(QtProClass);
		actionClear_list->setObjectName(QStringLiteral("actionClear_list"));
		actionOpen_Process = new QAction(QtProClass);
		actionOpen_Process->setObjectName(QStringLiteral("actionOpen_Process"));
		actionOpen_File = new QAction(QtProClass);
		actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
		actionSave = new QAction(QtProClass);
		actionSave->setObjectName(QStringLiteral("actionSave"));
		actionSaved_as = new QAction(QtProClass);
		actionSaved_as->setObjectName(QStringLiteral("actionSaved_as"));
		actionLoad = new QAction(QtProClass);
		actionLoad->setObjectName(QStringLiteral("actionLoad"));
		actionSave_current_scanresults = new QAction(QtProClass);
		actionSave_current_scanresults->setObjectName(QStringLiteral("actionSave_current_scanresults"));
		actionGenerate_generic_trainer_lua_script_from_table = new QAction(QtProClass);
		actionGenerate_generic_trainer_lua_script_from_table->setObjectName(QStringLiteral("actionGenerate_generic_trainer_lua_script_from_table"));
		actionQuit = new QAction(QtProClass);
		actionQuit->setObjectName(QStringLiteral("actionQuit"));
		actionSettings = new QAction(QtProClass);
		actionSettings->setObjectName(QStringLiteral("actionSettings"));
		actionShow_Cheat_Table_Lua_Script = new QAction(QtProClass);
		actionShow_Cheat_Table_Lua_Script->setObjectName(QStringLiteral("actionShow_Cheat_Table_Lua_Script"));
		actionCreate_Form = new QAction(QtProClass);
		actionCreate_Form->setObjectName(QStringLiteral("actionCreate_Form"));
		actionResynchronyze_forms_with_Lua = new QAction(QtProClass);
		actionResynchronyze_forms_with_Lua->setObjectName(QStringLiteral("actionResynchronyze_forms_with_Lua"));
		actionAdd_file = new QAction(QtProClass);
		actionAdd_file->setObjectName(QStringLiteral("actionAdd_file"));
		actionHook_Direct3D = new QAction(QtProClass);
		actionHook_Direct3D->setObjectName(QStringLiteral("actionHook_Direct3D"));
		actionSet_custom_CrossHair = new QAction(QtProClass);
		actionSet_custom_CrossHair->setObjectName(QStringLiteral("actionSet_custom_CrossHair"));
		actionLock_mouse_in_game_window = new QAction(QtProClass);
		actionLock_mouse_in_game_window->setObjectName(QStringLiteral("actionLock_mouse_in_game_window"));
		actionSnapshot_Handler = new QAction(QtProClass);
		actionSnapshot_Handler->setObjectName(QStringLiteral("actionSnapshot_Handler"));
		centralWidget = new QWidget(QtProClass);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		toolButton = new QToolButton(centralWidget);
		toolButton->setObjectName(QStringLiteral("toolButton"));
		toolButton->setGeometry(QRect(10, 20, 30, 30));
		toolButton->setIcon(QIcon("icons/Process.ico"));
		toolButton->setToolTip("Open a Process");
		toolButMod = new QToolButton(centralWidget);
		toolButMod->setObjectName(QStringLiteral("toolModulesButton"));
		toolButMod->setGeometry(QRect(50, 20, 30, 30));
		toolButMod->setIcon(QIcon("icons/Modules.ico"));
		toolButMod->setToolTip("Modules list");
		toolButSearch = new QToolButton(centralWidget);
		toolButSearch->setObjectName(QStringLiteral("SeachButton"));
		toolButSearch->setGeometry(QRect(90, 20, 30, 30));
		toolButSearch->setIcon(QIcon("icons/Search.ico"));
		toolButSearch->setToolTip("Search"); 
		toolButMemoryView = new QToolButton(centralWidget); 
		toolButMemoryView->setObjectName(QStringLiteral("Memory View"));
		toolButMemoryView->setGeometry(QRect(170, 20, 30, 30));
		toolButMemoryView->setIcon(QIcon("icons/MemoryView.ico"));
		toolButMemoryView->setToolTip("Memory View");
		toolButMemoryMap = new QToolButton(centralWidget);
		toolButMemoryMap->setObjectName(QStringLiteral("Memory Map"));
		toolButMemoryMap->setGeometry(QRect(210, 20, 30, 30));
		toolButMemoryMap->setIcon(QIcon("icons/MemoryMap.ico"));
		toolButMemoryMap->setToolTip("Memory Map");
		toolButThreads = new QToolButton(centralWidget);
		toolButThreads->setObjectName(QStringLiteral("ThreadsButton"));
		toolButThreads->setGeometry(QRect(250, 20, 30, 30));
		toolButThreads->setIcon(QIcon("icons/Threads.ico"));
		toolButThreads->setToolTip("Process Threads");
		toolButLogs = new QToolButton(centralWidget);
		toolButLogs->setObjectName(QStringLiteral("LogsButton"));
		toolButLogs->setGeometry(QRect(130, 20, 30, 30));
		toolButLogs->setIcon(QIcon("icons/Logs.ico"));
		toolButLogs->setToolTip("Process Logs");
		QtProClass->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(QtProClass);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 1054, 26));
		menu_File = new QMenu(menuBar);
		menu_File->setObjectName(QStringLiteral("menu_File"));
		menuEdit = new QMenu(menuBar);
		menuEdit->setObjectName(QStringLiteral("menuEdit"));
		menuTable = new QMenu(menuBar);
		menuTable->setObjectName(QStringLiteral("menuTable"));
		menuD3D = new QMenu(menuBar);
		menuD3D->setObjectName(QStringLiteral("menuD3D"));
		menuHelp = new QMenu(menuBar);
		menuHelp->setObjectName(QStringLiteral("menuHelp"));
		QtProClass->setMenuBar(menuBar);
		statusBar = new QStatusBar(QtProClass);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		QtProClass->setStatusBar(statusBar);
		lblProcessName = new QLabel(centralWidget);
		lblProcessName->setObjectName(QStringLiteral("lblValue"));
		lblProcessName->setGeometry(QRect(400, 0, 261, 16));
		menuBar->addAction(menu_File->menuAction());
		menuBar->addAction(menuEdit->menuAction());
		menuBar->addAction(menuTable->menuAction());
		menuBar->addAction(menuD3D->menuAction());
		menuBar->addAction(menuHelp->menuAction());
		menu_File->addAction(actionAdd_Scan_Table);
		menu_File->addAction(actionClear_list);
		menu_File->addAction(actionOpen_Process);
		menu_File->addAction(actionOpen_File);
		menu_File->addSeparator();
		menu_File->addAction(actionSave);
		menu_File->addAction(actionSaved_as);
		menu_File->addAction(actionLoad);
		menu_File->addSeparator();
		menu_File->addAction(actionSave_current_scanresults);
		menu_File->addSeparator();
		menu_File->addAction(actionGenerate_generic_trainer_lua_script_from_table);
		menu_File->addSeparator();
		menu_File->addAction(actionQuit);
		menuEdit->addAction(actionSettings);
		menuTable->addAction(actionShow_Cheat_Table_Lua_Script);
		menuTable->addSeparator();
		menuTable->addAction(actionCreate_Form);
		menuTable->addAction(actionResynchronyze_forms_with_Lua);
		menuTable->addSeparator();
		menuTable->addAction(actionAdd_file);
		menuD3D->addAction(actionHook_Direct3D);
		menuD3D->addSeparator();
		menuD3D->addAction(actionSet_custom_CrossHair);
		menuD3D->addAction(actionLock_mouse_in_game_window);
		menuD3D->addAction(actionSnapshot_Handler);
		menuHelp->addAction(actionCheat_Engine_Help_F1);
		menuHelp->addAction(actionCheat_Engine_Tutorial);
		menuHelp->addSeparator();
		menuHelp->addAction(actionAbout);

		retranslateUi(QtProClass);
		QObject::connect(actionQuit, SIGNAL(triggered()), QtProClass, SLOT(close()));

		QMetaObject::connectSlotsByName(QtProClass);
	} // setupUi

	void retranslateUi(QMainWindow *QtProClass)
	{
		QtProClass->setWindowTitle(QApplication::translate("QtProClass", "Cpp Engine", 0));
		action_Exit->setText(QApplication::translate("QtProClass", "&Exit", 0));
		actionE_xit->setText(QApplication::translate("QtProClass", "E&xit", 0));
		actionCheat_Engine_Help_F1->setText(QApplication::translate("QtProClass", "Cheat Engine Help F1", 0));
		actionCheat_Engine_Tutorial->setText(QApplication::translate("QtProClass", "Cheat Engine Tutorial", 0));
		actionAbout->setText(QApplication::translate("QtProClass", "&About", 0));
		actionAdd_Scan_Table->setText(QApplication::translate("QtProClass", "Add scan tab", 0));
		actionClear_list->setText(QApplication::translate("QtProClass", "Clear list", 0));
		actionOpen_Process->setText(QApplication::translate("QtProClass", "Open Process", 0));
		actionOpen_File->setText(QApplication::translate("QtProClass", "Open File", 0));
		actionSave->setText(QApplication::translate("QtProClass", "Save", 0));
		actionSaved_as->setText(QApplication::translate("QtProClass", "Saved as..", 0));
		actionLoad->setText(QApplication::translate("QtProClass", "Load", 0));
		actionSave_current_scanresults->setText(QApplication::translate("QtProClass", "Save current scanresults", 0));
		actionGenerate_generic_trainer_lua_script_from_table->setText(QApplication::translate("QtProClass", "Generate generic trainer lua script from table", 0));
		actionQuit->setText(QApplication::translate("QtProClass", "Quit", 0));
		actionSettings->setText(QApplication::translate("QtProClass", "Settings", 0));
		actionShow_Cheat_Table_Lua_Script->setText(QApplication::translate("QtProClass", "Show Cheat Table Lua Script", 0));
		actionCreate_Form->setText(QApplication::translate("QtProClass", "Create Form", 0));
		actionResynchronyze_forms_with_Lua->setText(QApplication::translate("QtProClass", "Resynchronize forms with Lua", 0));
		actionAdd_file->setText(QApplication::translate("QtProClass", "Add file", 0));
		actionHook_Direct3D->setText(QApplication::translate("QtProClass", "Hook Direct3D", 0));
		actionSet_custom_CrossHair->setText(QApplication::translate("QtProClass", "Set custom CrossHair", 0));
		actionLock_mouse_in_game_window->setText(QApplication::translate("QtProClass", "Lock mouse in game window", 0));
		actionSnapshot_Handler->setText(QApplication::translate("QtProClass", "Snapshot Handler", 0));
		lblProcessName->setText(QApplication::translate("QtProClass", "Process Name", 0));
		menu_File->setTitle(QApplication::translate("QtProClass", "&File", 0));
		menuEdit->setTitle(QApplication::translate("QtProClass", "&Edit", 0));
		menuTable->setTitle(QApplication::translate("QtProClass", "&Table", 0));
		menuD3D->setTitle(QApplication::translate("QtProClass", "&D3D", 0));
		menuHelp->setTitle(QApplication::translate("QtProClass", "&Help", 0));
	} // retranslateUi

};

namespace Ui {
	class QtProClass : public Ui_QtProClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTPRO_H
