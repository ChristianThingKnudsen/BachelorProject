/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayoutRegion;
    QLabel *labelRegion;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayoutLoadCT;
    QPushButton *btnLoadCT;
    QCheckBox *cbCT;
    QHBoxLayout *horizontalLayoutLoadCBCT;
    QPushButton *btnLoadCBCT;
    QCheckBox *cbCBCT;
    QHBoxLayout *horizontalLayoutSC;
    QPushButton *btnScatterCorrect;
    QHBoxLayout *horizontalLayoutID;
    QLabel *label_Id;
    QCheckBox *cbShowOrgans;
    QPushButton *btnInfo;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QPushButton *pushButton;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(968, 557);
        MainWindow->setStyleSheet(QString::fromUtf8("MainWindow{\n"
"	background-color: #005194\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayoutRegion = new QHBoxLayout();
        horizontalLayoutRegion->setObjectName(QString::fromUtf8("horizontalLayoutRegion"));
        labelRegion = new QLabel(centralwidget);
        labelRegion->setObjectName(QString::fromUtf8("labelRegion"));

        horizontalLayoutRegion->addWidget(labelRegion);

        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayoutRegion->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayoutRegion);

        horizontalLayoutLoadCT = new QHBoxLayout();
        horizontalLayoutLoadCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCT"));
        btnLoadCT = new QPushButton(centralwidget);
        btnLoadCT->setObjectName(QString::fromUtf8("btnLoadCT"));
        btnLoadCT->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCT->addWidget(btnLoadCT);

        cbCT = new QCheckBox(centralwidget);
        cbCT->setObjectName(QString::fromUtf8("cbCT"));
        cbCT->setAutoFillBackground(false);
        cbCT->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	color: #FF0000\n"
"}"));
        cbCT->setChecked(false);

        horizontalLayoutLoadCT->addWidget(cbCT);


        verticalLayout->addLayout(horizontalLayoutLoadCT);

        horizontalLayoutLoadCBCT = new QHBoxLayout();
        horizontalLayoutLoadCBCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCBCT"));
        btnLoadCBCT = new QPushButton(centralwidget);
        btnLoadCBCT->setObjectName(QString::fromUtf8("btnLoadCBCT"));
        btnLoadCBCT->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCBCT->addWidget(btnLoadCBCT);

        cbCBCT = new QCheckBox(centralwidget);
        cbCBCT->setObjectName(QString::fromUtf8("cbCBCT"));
        cbCBCT->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	check-color: #FF0000;\n"
"}"));

        horizontalLayoutLoadCBCT->addWidget(cbCBCT);


        verticalLayout->addLayout(horizontalLayoutLoadCBCT);

        horizontalLayoutSC = new QHBoxLayout();
        horizontalLayoutSC->setObjectName(QString::fromUtf8("horizontalLayoutSC"));
        btnScatterCorrect = new QPushButton(centralwidget);
        btnScatterCorrect->setObjectName(QString::fromUtf8("btnScatterCorrect"));
        btnScatterCorrect->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutSC->addWidget(btnScatterCorrect);


        verticalLayout->addLayout(horizontalLayoutSC);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 2, 1);

        horizontalLayoutID = new QHBoxLayout();
        horizontalLayoutID->setObjectName(QString::fromUtf8("horizontalLayoutID"));
        label_Id = new QLabel(centralwidget);
        label_Id->setObjectName(QString::fromUtf8("label_Id"));

        horizontalLayoutID->addWidget(label_Id);

        cbShowOrgans = new QCheckBox(centralwidget);
        cbShowOrgans->setObjectName(QString::fromUtf8("cbShowOrgans"));

        horizontalLayoutID->addWidget(cbShowOrgans);

        btnInfo = new QPushButton(centralwidget);
        btnInfo->setObjectName(QString::fromUtf8("btnInfo"));
        btnInfo->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:#E4A115; \n"
"	color: #ffffff;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color:#1367AB;\n"
"}"));

        horizontalLayoutID->addWidget(btnInfo);


        gridLayout_2->addLayout(horizontalLayoutID, 0, 1, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        gridLayout_2->addLayout(gridLayout, 1, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        verticalLayout_2->addWidget(pushButton);


        gridLayout_2->addLayout(verticalLayout_2, 1, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(btnLoadCBCT, SIGNAL(released()), MainWindow, SLOT(foo()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelRegion->setText(QCoreApplication::translate("MainWindow", "Region:", nullptr));
        btnLoadCT->setText(QCoreApplication::translate("MainWindow", "Load CT", nullptr));
        cbCT->setText(QString());
        btnLoadCBCT->setText(QCoreApplication::translate("MainWindow", "Load CBCT", nullptr));
        cbCBCT->setText(QString());
        btnScatterCorrect->setText(QCoreApplication::translate("MainWindow", "Scatter Correct", nullptr));
        label_Id->setText(QCoreApplication::translate("MainWindow", "ID:xxxxxx-xxxx", nullptr));
        cbShowOrgans->setText(QCoreApplication::translate("MainWindow", "Show Organs", nullptr));
        btnInfo->setText(QCoreApplication::translate("MainWindow", "Information", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Advanced mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
