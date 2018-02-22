/********************************************************************************
** Form generated from reading UI file 'Search107952.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef SEARCH107952_H
#define SEARCH107952_H
#include <QtWidgets/QDialogButtonBox>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QSplitter>
#include <QHeaderView>
#include <vector>
#include <cstdlib>
QT_BEGIN_NAMESPACE

class Ui_DialogSearch
{
public:
	QDialog *_Dialog;
	QDialogButtonBox *OkCancelBox;
	QWidget *widget;
	QHBoxLayout *horizontalLayout;
	QLabel *label;
	QCheckBox *cbHex;
	QLineEdit *LineScanValue;
	QWidget *widget1;
	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout_3;
	QLabel *label_2;
	QComboBox *comboBScanType;
	QHBoxLayout *horizontalLayout_2;
	QLabel *label_3;
	QComboBox *comboBValueType;
	QWidget *widget2;
	QHBoxLayout *horizontalLayout_4;
	QCheckBox *cbExecutable;
	QCheckBox *cbWritable;
	QCheckBox *cbCopyOnWrite;
	QWidget *widget3;
	QVBoxLayout *verticalLayout_2;
	QCheckBox *cbFastScan;
	QCheckBox *cbAlwaysOnTop;
	QWidget *widget4;
	QVBoxLayout *verticalLayout_3;
	QHBoxLayout *horizontalLayout_5;
	QLabel *lblRangeStart;
	QLineEdit *lineRangeStart;
	QHBoxLayout *horizontalLayout_6;
	QSpacerItem *horizontalSpacer;
	QLabel *lblRangeEnd;
	QLineEdit *lineRangeEnd;
	QWidget *widget5;
	QHBoxLayout *horizontalLayout_7;
	QPushButton *pbFirstNewScan;
	QPushButton *pbNextScan;
	void Presets()
	{
		cbExecutable->setCheckState(Qt::PartiallyChecked);
		cbWritable->setChecked(true);
		cbCopyOnWrite->setChecked(false);
		cbAlwaysOnTop->setChecked(false);
		cbFastScan->setDisabled(false);
		pbNextScan->setDisabled(true);
		lineRangeStart->setText("0000000000000000");
		lineRangeEnd->setText("7FFFFFFFFFFFFFFF");
		comboBScanType->setCurrentIndex(0);
		comboBValueType->setCurrentIndex(3);
		LineScanValue->setFocus();
		QWidget::setTabOrder(pbFirstNewScan, pbNextScan);
		QWidget::setTabOrder(pbNextScan, OkCancelBox);
		QWidget::setTabOrder(OkCancelBox, comboBScanType);
		QWidget::setTabOrder(comboBScanType, cbHex);
	}
	void setupUi(QDialog *Dialog)
	{
		_Dialog = Dialog;
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Dialog"));
		Dialog->setWindowIcon(QIcon("icons/CppEngine.ico"));
		Dialog->setFixedSize(390, 249);
		OkCancelBox = new QDialogButtonBox(Dialog);
		OkCancelBox->setObjectName(QStringLiteral("buttonBox"));
		OkCancelBox->setGeometry(QRect(190, 220, 193, 28));
		OkCancelBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
		widget = new QWidget(Dialog);
		widget->setObjectName(QStringLiteral("widget"));
		widget->setGeometry(QRect(130, 10, 238, 24));
		horizontalLayout = new QHBoxLayout(widget);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		horizontalLayout->setContentsMargins(0, 0, 0, 0);
		label = new QLabel(widget);
		label->setObjectName(QStringLiteral("label"));

		horizontalLayout->addWidget(label);

		cbHex = new QCheckBox(widget);
		cbHex->setObjectName(QStringLiteral("checkBox"));

		horizontalLayout->addWidget(cbHex);

		LineScanValue = new QLineEdit(widget);
		LineScanValue->setObjectName(QStringLiteral("lineEdit"));

		horizontalLayout->addWidget(LineScanValue);

		widget1 = new QWidget(Dialog);
		widget1->setObjectName(QStringLiteral("widget1"));
		widget1->setGeometry(QRect(164, 40, 215, 57));
		verticalLayout = new QVBoxLayout(widget1);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		horizontalLayout_3 = new QHBoxLayout();
		horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
		label_2 = new QLabel(widget1);
		label_2->setObjectName(QStringLiteral("label_2"));

		horizontalLayout_3->addWidget(label_2);

		comboBScanType = new QComboBox(widget1);
		comboBScanType->setObjectName(QStringLiteral("comboBox_2"));
		comboBScanType->addItems(QStringList() << "Exact Value" << "Bigger than.." << "Smaller than.." << "Unknown initial value");

		horizontalLayout_3->addWidget(comboBScanType);

		verticalLayout->addLayout(horizontalLayout_3);

		horizontalLayout_2 = new QHBoxLayout();
		horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
		label_3 = new QLabel(widget1);
		label_3->setObjectName(QStringLiteral("label_3"));

		horizontalLayout_2->addWidget(label_3);

		comboBValueType = new QComboBox(widget1);
		comboBValueType->setObjectName(QStringLiteral("comboBox"));
		comboBValueType->addItems(QStringList() << "Bynary" << "Byte" << "2 Bytes" << "4 Bytes" << "8 Bytes" << "Float" << "Double" << "String" << "Array of bytes");

		horizontalLayout_2->addWidget(comboBValueType);


		verticalLayout->addLayout(horizontalLayout_2);

		widget2 = new QWidget(Dialog);
		widget2->setObjectName(QStringLiteral("widget2"));
		widget2->setGeometry(QRect(90, 110, 281, 22));
		horizontalLayout_4 = new QHBoxLayout(widget2);
		horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
		horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
		cbExecutable = new QCheckBox(widget2);
		cbExecutable->setObjectName(QStringLiteral("checkBox_2"));
		cbExecutable->setTristate(true);
		horizontalLayout_4->addWidget(cbExecutable);

		cbWritable = new QCheckBox(widget2);
		cbWritable->setObjectName(QStringLiteral("checkBox_3"));
		cbWritable->setTristate(true);
		cbWritable->setToolTip("Only scans writable memory regions when checked");
		horizontalLayout_4->addWidget(cbWritable);

		cbCopyOnWrite = new QCheckBox(widget2);
		cbCopyOnWrite->setObjectName(QStringLiteral("checkBox_4"));
		cbCopyOnWrite->setTristate(true);
		horizontalLayout_4->addWidget(cbCopyOnWrite);

		widget3 = new QWidget(Dialog);
		widget3->setObjectName(QStringLiteral("widget3"));
		widget3->setGeometry(QRect(0, 200, 109, 49));
		verticalLayout_2 = new QVBoxLayout(widget3);
		verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
		verticalLayout_2->setContentsMargins(0, 0, 0, 0);
		cbFastScan = new QCheckBox(widget3);
		cbFastScan->setObjectName(QStringLiteral("checkBox_5"));

		verticalLayout_2->addWidget(cbFastScan);

		cbAlwaysOnTop = new QCheckBox(widget3);
		cbAlwaysOnTop->setObjectName(QStringLiteral("checkBox_6"));

		verticalLayout_2->addWidget(cbAlwaysOnTop);

		widget4 = new QWidget(Dialog);
		widget4->setObjectName(QStringLiteral("widget4"));
		widget4->setGeometry(QRect(80, 140, 311, 57));
		verticalLayout_3 = new QVBoxLayout(widget4);
		verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
		verticalLayout_3->setContentsMargins(0, 0, 0, 0);
		horizontalLayout_5 = new QHBoxLayout();
		horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
		lblRangeStart = new QLabel(widget4);
		lblRangeStart->setObjectName(QStringLiteral("label_4"));

		horizontalLayout_5->addWidget(lblRangeStart);

		lineRangeStart = new QLineEdit(widget4);
		lineRangeStart->setObjectName(QStringLiteral("lineEdit_2"));

		horizontalLayout_5->addWidget(lineRangeStart);


		verticalLayout_3->addLayout(horizontalLayout_5);

		horizontalLayout_6 = new QHBoxLayout();
		horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
		horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

		horizontalLayout_6->addItem(horizontalSpacer);

		lblRangeEnd = new QLabel(widget4);
		lblRangeEnd->setObjectName(QStringLiteral("label_5"));

		horizontalLayout_6->addWidget(lblRangeEnd);

		lineRangeEnd = new QLineEdit(widget4);
		lineRangeEnd->setObjectName(QStringLiteral("lineEdit_3"));

		horizontalLayout_6->addWidget(lineRangeEnd);


		verticalLayout_3->addLayout(horizontalLayout_6);

		widget5 = new QWidget(Dialog);
		widget5->setObjectName(QStringLiteral("widget5"));
		widget5->setGeometry(QRect(5, 50, 155, 25));
		horizontalLayout_7 = new QHBoxLayout(widget5);
		horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
		horizontalLayout_7->setContentsMargins(0, 0, 0, 0);
		pbFirstNewScan = new QPushButton(widget5);
		pbFirstNewScan->setObjectName(QStringLiteral("pushButton"));

		horizontalLayout_7->addWidget(pbFirstNewScan);

		pbNextScan = new QPushButton(widget5);
		pbNextScan->setObjectName(QStringLiteral("pushButton_2"));

		horizontalLayout_7->addWidget(pbNextScan);

		retranslateUi(Dialog);
		QObject::connect(OkCancelBox, SIGNAL(rejected()), Dialog, SLOT(reject()));
		QObject::connect(OkCancelBox, SIGNAL(accepted()), Dialog, SLOT(accept()));
		QMetaObject::connectSlotsByName(Dialog);
		Presets();
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "Scan Box", 0));
		label->setText(QApplication::translate("Dialog", "Value:", 0));
		cbHex->setText(QApplication::translate("Dialog", "Hex", 0));
		label_2->setText(QApplication::translate("Dialog", "Scan Type:", 0));
		label_3->setText(QApplication::translate("Dialog", "Value Type:", 0));
		cbExecutable->setText(QApplication::translate("Dialog", "Executable", 0));
		cbWritable->setText(QApplication::translate("Dialog", "Writable", 0));
		cbCopyOnWrite->setText(QApplication::translate("Dialog", "CopyOnWrite", 0));
		cbFastScan->setText(QApplication::translate("Dialog", "Fast scan", 0));
		cbAlwaysOnTop->setText(QApplication::translate("Dialog", "Always on top", 0));
		lblRangeStart->setText(QApplication::translate("Dialog", "Memory range scan : Start :", 0));
		lblRangeEnd->setText(QApplication::translate("Dialog", "End :", 0));
		pbFirstNewScan->setText(QApplication::translate("Dialog", "First Scan", 0));
		pbNextScan->setText(QApplication::translate("Dialog", "Next Scan", 0));
	} // retranslateUi
	/*Exists in both Ui::Search and Dialog and is called by another one's destructor so that closing one of them closes the other.*/
};


class Ui_DialogResults
{
public:
	QDialog *_Dialog;
	QWidget *widget;
	QVBoxLayout *verticalLayout;
	QHBoxLayout *horizontalLayout;
	QHBoxLayout* horizontalLayout_2;
	QLabel *label;
	QLabel *label_2;
	QSplitter *splitter;
	QTreeWidget *treeWidget;
	QTreeWidget *treeWidget_2;
	QTreeWidgetItem * itm;
	std::vector <uint64_t> _vecResultsAddr;
	std::vector <uint64_t> _vecSavedAddr;
	void setupUi(QDialog *Dialog)
	{
		_Dialog = Dialog;
		if (Dialog->objectName().isEmpty())
			Dialog->setObjectName(QStringLiteral("Dialog"));
		Dialog->resize(800, 450);
		Dialog->setWindowIcon(QIcon("icons/CppEngine.ico"));
		splitter = new QSplitter(Dialog);
		splitter->setObjectName(QStringLiteral("splitter"));
		splitter->setGeometry(QRect(0, 0, 620, 219));
		splitter->setOrientation(Qt::Horizontal);
		widget = new QWidget(splitter);
		widget->setObjectName(QStringLiteral("widget"));
		//widget->setGeometry(QRect(0, 0, 523, 289));
		verticalLayout = new QVBoxLayout(widget);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		verticalLayout->setContentsMargins(0, 0, 0, 0);
		horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		horizontalLayout_2 = new QHBoxLayout();
		label = new QLabel(widget);
		label->setObjectName(QStringLiteral("label"));

		horizontalLayout->addWidget(label);

		label_2 = new QLabel(widget);
		label_2->setObjectName(QStringLiteral("label_2"));

		horizontalLayout->addWidget(label_2);


		verticalLayout->addLayout(horizontalLayout);

		treeWidget = new QTreeWidget(widget);
		treeWidget->setObjectName(QStringLiteral("tableWidget"));
		treeWidget->setHeaderLabels(QStringList() << "Address" << "Value");
		treeWidget->setColumnWidth(0, 160);
		treeWidget->setColumnWidth(1, 160);
		treeWidget->setRootIsDecorated(false);
		verticalLayout->addWidget(treeWidget);
		treeWidget_2 = new QTreeWidget(splitter);
		treeWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
		treeWidget_2->setHeaderLabels(QStringList() << "Description" << "Address" << "Value" << "Type");
		treeWidget_2->setColumnWidth(0, 120);
		treeWidget_2->setColumnWidth(1, 120);
		treeWidget_2->setColumnWidth(2, 120);
		treeWidget_2->setColumnWidth(3, 120);
		treeWidget_2->setRootIsDecorated(false);
		QList<int> sizes;
		sizes << 300 << 500;
		splitter->setSizes(sizes);
		splitter->addWidget(widget);
		splitter->addWidget(treeWidget_2);
		horizontalLayout_2->addWidget(splitter);
		horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
		Dialog->setLayout(horizontalLayout_2);
		retranslateUi(Dialog);
		QMetaObject::connectSlotsByName(Dialog);
	} // setupUi

	void retranslateUi(QDialog *Dialog)
	{
		Dialog->setWindowTitle(QApplication::translate("Dialog", "Results", 0));
		label->setText(QApplication::translate("Dialog", "Search Results : ", 0));
		label_2->setText(QApplication::translate("Dialog", "0", 0));
	} // retranslateUi
};

namespace Ui {
	class Ui_Search : public Ui_DialogSearch {};
	class Ui_Results : public Ui_DialogResults {};
} // namespace Ui

QT_END_NAMESPACE

#endif // SEARCH107952_H
