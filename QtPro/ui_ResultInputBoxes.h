#pragma once
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
#include <QShortcut>
#include "qtpro.h"
#include "DebuggedProcess.h"
class Ui_DialogCommentBox
{
public:
	QDialogButtonBox *buttonBox;
	QLineEdit *textEdit;
	QLabel * label;
public:
	void setupUi(QDialog *Dialog, QTreeWidgetItem * itm, int column)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Goto Address"));
		Dialog->resize(377, 92);
		buttonBox = new QDialogButtonBox(Dialog);
		buttonBox->setObjectName(QStringLiteral("buttonBox"));
		buttonBox->setGeometry(QRect(290, 20, 81, 241));
		buttonBox->setOrientation(Qt::Vertical);
		buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
		textEdit = new QLineEdit(Dialog);
		textEdit->setObjectName(QStringLiteral("textEdit"));
		textEdit->setGeometry(QRect(20, 40, 241, 31));
		label = new QLabel(Dialog);
		label->setObjectName(QStringLiteral("label"));
		label->setGeometry(QRect(30, 0, 221, 31));

		retranslateUi(Dialog, column);
		//QObject::connect(buttonBox, &buttonBox->clicked, this, &this->foo);
		//QObject::connect(buttonBox, &QDialogButtonBox::clicked, this, &Ui_DialogGotoBox::foo);
		//QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(close()));
		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog, int column)
	{
		switch (column)
		{
		case 0:
			Dialog->setWindowTitle(QApplication::translate("Dialog", "Change Description", 0));
			label->setText(QApplication::translate("Dialog", "Enter the new description :", 0));
			break;
		case 2:
			Dialog->setWindowTitle(QApplication::translate("Dialog", "Change Value", 0));
			label->setText(QApplication::translate("Dialog", "Enter the new value :", 0));
			break;
		}
	}

};
class Ui_DialogTypeBox
{
public:
	QWidget *widget;
	QVBoxLayout *verticalLayout;
	QLabel *label;
	QComboBox *comboBValueType;
	QDialogButtonBox *buttonBox;
	void presets ()
	{
		comboBValueType->setCurrentIndex(3);
	}
	void setupUi(QDialog *Dialog, QTreeWidgetItem * itm, int column)
	{
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Dialog"));
		Dialog->resize(209, 97);
		Dialog->setFixedSize(209, 97);
		widget = new QWidget(Dialog);
		widget->setObjectName(QStringLiteral("widget"));
		widget->setGeometry(QRect(10, 10, 195, 82));
		verticalLayout = new QVBoxLayout(widget);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		label = new QLabel(widget);
		label->setObjectName(QStringLiteral("label"));

		verticalLayout->addWidget(label);

		comboBValueType = new QComboBox(widget);
		comboBValueType->setObjectName(QStringLiteral("comboBox"));
		comboBValueType->addItems(QStringList() << "Bynary" << "Byte" << "2 Bytes" << "4 Bytes" << "8 Bytes" << "Float" << "Double" << "String" << "Array of bytes");
		verticalLayout->addWidget(comboBValueType);

		buttonBox = new QDialogButtonBox(widget);
		buttonBox->setObjectName(QStringLiteral("buttonBox"));
		buttonBox->setOrientation(Qt::Horizontal);
		buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);

		verticalLayout->addWidget(buttonBox);

		presets();
		retranslateUi(Dialog);
		QObject::connect(buttonBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
		QObject::connect(buttonBox, SIGNAL(rejected()), Dialog, SLOT(reject()));

		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "Change Type", 0));
		label->setText(QApplication::translate("Dialog", "Select the new type", 0));
	} // retranslateUi

};


namespace Ui{
class CommentBox : public Ui_DialogCommentBox {};
class TypeBox : public Ui_DialogTypeBox {};
}