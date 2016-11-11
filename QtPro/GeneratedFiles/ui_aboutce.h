/********************************************************************************
** Form generated from reading UI file 'aboutce.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTCE_H
#define UI_ABOUTCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_aboutCEClass
{
public:
    QPushButton *pushButton;
    QLabel *label;
    QLabel *label_2;
	QAction * actionQuit;
    void setupUi(QDialog *aboutCEClass)
    {
        if (aboutCEClass->objectName().isEmpty())
            aboutCEClass->setObjectName(QStringLiteral("aboutdCEClass"));
		aboutCEClass->resize(327, 292);
		aboutCEClass->setFixedSize(327, 292);
		actionQuit = new QAction(aboutCEClass);
        pushButton = new QPushButton(aboutCEClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(140, 210, 41, 31));
		//pushButton->addAction(actionQuit);
        label = new QLabel(aboutCEClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 80, 291, 21));
        label_2 = new QLabel(aboutCEClass);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 150, 161, 21));
		QObject::connect(pushButton, SIGNAL(clicked()), aboutCEClass, SLOT(close()));
        retranslateUi(aboutCEClass);

        QMetaObject::connectSlotsByName(aboutCEClass);
    } // setupUi

    void retranslateUi(QDialog *aboutCEClass)
    {
        aboutCEClass->setWindowTitle(QApplication::translate("aboutCEClass", "About Ulysse Engine", 0));
        pushButton->setText(QApplication::translate("aboutCEClass", "Ok", 0));
        label->setText(QApplication::translate("aboutCEClass", "This program aims to be a c++ version like of CE", 0));
        label_2->setText(QApplication::translate("aboutCEClass", "All credits go to DarkByte", 0));
    } // retranslateUi

};

namespace Ui {
    class AboutCEClass: public Ui_aboutCEClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTCE_H
