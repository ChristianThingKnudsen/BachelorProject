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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qyklabel.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QFrame *frame1;
    QHBoxLayout *horizontalLayoutRegion;
    QLabel *labelRegion;
    QComboBox *comboBox_region;
    QFrame *frame2;
    QHBoxLayout *horizontalLayoutLoadCT;
    QPushButton *btnLoadData;
    QCheckBox *cbCT;
    QFrame *frame3;
    QHBoxLayout *horizontalLayoutLoadCBCT;
    QPushButton *btnScatterCorrect;
    QFrame *frame4;
    QHBoxLayout *horizontalLayoutSC;
    QPushButton *btnAdvanced;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame5;
    QGridLayout *gridLayout;
    qyklabel *labelImageRaw;
    QFrame *frame6;
    QHBoxLayout *horizontalLayoutID;
    QLabel *label_Id;
    QCheckBox *cbShowOrgans;
    QPushButton *btnInfo;
    QPushButton *btnExit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(976, 535);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setStyleSheet(QString::fromUtf8("MainWindow{\n"
"	background-color: #1367AB;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setVerticalSpacing(3);
        gridLayout_2->setContentsMargins(3, 6, 3, 6);
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 0, 1, 0);
        frame1 = new QFrame(frame);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: #0D3F64;\n"
"}"));
        horizontalLayoutRegion = new QHBoxLayout(frame1);
        horizontalLayoutRegion->setObjectName(QString::fromUtf8("horizontalLayoutRegion"));
        horizontalLayoutRegion->setContentsMargins(9, -1, -1, -1);
        labelRegion = new QLabel(frame1);
        labelRegion->setObjectName(QString::fromUtf8("labelRegion"));
        labelRegion->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 20px;\n"
"	color:#ffffff;\n"
"}"));

        horizontalLayoutRegion->addWidget(labelRegion);

        comboBox_region = new QComboBox(frame1);
        comboBox_region->setObjectName(QString::fromUtf8("comboBox_region"));
        comboBox_region->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"	font-weight: bold;\n"
"	font-size: 18px;\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"}"));

        horizontalLayoutRegion->addWidget(comboBox_region);


        verticalLayout->addWidget(frame1);

        frame2 = new QFrame(frame);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutLoadCT = new QHBoxLayout(frame2);
        horizontalLayoutLoadCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCT"));
        btnLoadData = new QPushButton(frame2);
        btnLoadData->setObjectName(QString::fromUtf8("btnLoadData"));
        btnLoadData->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: qlineargradient(spread:reflect, x1:0, y1:1, x2:0.5, y2:1, stop:0 rgba(0, 0, 0, 255), stop:0.724771 rgba(209, 209, 209, 255));\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCT->addWidget(btnLoadData);

        cbCT = new QCheckBox(frame2);
        cbCT->setObjectName(QString::fromUtf8("cbCT"));
        cbCT->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbCT->sizePolicy().hasHeightForWidth());
        cbCT->setSizePolicy(sizePolicy1);
        cbCT->setAutoFillBackground(false);
        cbCT->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	background-color: #FF0000;\n"
"}"));
        cbCT->setCheckable(true);
        cbCT->setAutoExclusive(false);

        horizontalLayoutLoadCT->addWidget(cbCT);


        verticalLayout->addWidget(frame2);

        frame3 = new QFrame(frame);
        frame3->setObjectName(QString::fromUtf8("frame3"));
        frame3->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutLoadCBCT = new QHBoxLayout(frame3);
        horizontalLayoutLoadCBCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCBCT"));
        btnScatterCorrect = new QPushButton(frame3);
        btnScatterCorrect->setObjectName(QString::fromUtf8("btnScatterCorrect"));
        btnScatterCorrect->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCBCT->addWidget(btnScatterCorrect);


        verticalLayout->addWidget(frame3);

        frame4 = new QFrame(frame);
        frame4->setObjectName(QString::fromUtf8("frame4"));
        frame4->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutSC = new QHBoxLayout(frame4);
        horizontalLayoutSC->setObjectName(QString::fromUtf8("horizontalLayoutSC"));
        horizontalLayoutSC->setContentsMargins(-1, -1, -1, 9);
        btnAdvanced = new QPushButton(frame4);
        btnAdvanced->setObjectName(QString::fromUtf8("btnAdvanced"));
        btnAdvanced->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutSC->addWidget(btnAdvanced);


        verticalLayout->addWidget(frame4);


        gridLayout_2->addWidget(frame, 0, 0, 2, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        gridLayout_2->addLayout(verticalLayout_4, 3, 2, 1, 1);

        frame5 = new QFrame(centralwidget);
        frame5->setObjectName(QString::fromUtf8("frame5"));
        frame5->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        gridLayout = new QGridLayout(frame5);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setContentsMargins(9, 9, -1, -1);
        labelImageRaw = new qyklabel(frame5);
        labelImageRaw->setObjectName(QString::fromUtf8("labelImageRaw"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelImageRaw->sizePolicy().hasHeightForWidth());
        labelImageRaw->setSizePolicy(sizePolicy2);
        labelImageRaw->setMinimumSize(QSize(256, 256));
        labelImageRaw->setMaximumSize(QSize(1024, 1024));
        labelImageRaw->setMouseTracking(true);
        labelImageRaw->setFrameShape(QFrame::Box);

        gridLayout->addWidget(labelImageRaw, 0, 0, 1, 1);


        gridLayout_2->addWidget(frame5, 1, 1, 1, 1);

        frame6 = new QFrame(centralwidget);
        frame6->setObjectName(QString::fromUtf8("frame6"));
        frame6->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutID = new QHBoxLayout(frame6);
        horizontalLayoutID->setObjectName(QString::fromUtf8("horizontalLayoutID"));
        horizontalLayoutID->setContentsMargins(9, -1, -1, -1);
        label_Id = new QLabel(frame6);
        label_Id->setObjectName(QString::fromUtf8("label_Id"));
        label_Id->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 20px;\n"
"}"));

        horizontalLayoutID->addWidget(label_Id);

        cbShowOrgans = new QCheckBox(frame6);
        cbShowOrgans->setObjectName(QString::fromUtf8("cbShowOrgans"));
        cbShowOrgans->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	font-size: 20px;\n"
"	color: #ffffff;\n"
"}"));

        horizontalLayoutID->addWidget(cbShowOrgans);

        btnInfo = new QPushButton(frame6);
        btnInfo->setObjectName(QString::fromUtf8("btnInfo"));
        btnInfo->setMinimumSize(QSize(40, 40));
        btnInfo->setMaximumSize(QSize(40, 40));
        btnInfo->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:#E4A115; \n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color:#1367AB;\n"
"}"));

        horizontalLayoutID->addWidget(btnInfo);

        btnExit = new QPushButton(frame6);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        sizePolicy1.setHeightForWidth(btnExit->sizePolicy().hasHeightForWidth());
        btnExit->setSizePolicy(sizePolicy1);
        btnExit->setMinimumSize(QSize(40, 40));
        btnExit->setMaximumSize(QSize(40, 40));
        btnExit->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #FF0000;\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}"));

        horizontalLayoutID->addWidget(btnExit);


        gridLayout_2->addWidget(frame6, 0, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        QObject::connect(btnScatterCorrect, SIGNAL(pressed()), MainWindow, SLOT(SLT_DoRegistrationRigid()));
        QObject::connect(btnAdvanced, SIGNAL(pressed()), MainWindow, SLOT(SLT_OpenAdvancedMode()));
        QObject::connect(btnLoadData, SIGNAL(pressed()), MainWindow, SLOT(SLT_SetHisDir()));
        QObject::connect(btnInfo, SIGNAL(pressed()), MainWindow, SLOT(SLT_OpenInfo()));
        QObject::connect(btnExit, SIGNAL(pressed()), MainWindow, SLOT(SLT_Exit()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelRegion->setText(QCoreApplication::translate("MainWindow", "Region:", nullptr));
        btnLoadData->setText(QCoreApplication::translate("MainWindow", "Load Data", nullptr));
        cbCT->setText(QString());
        btnScatterCorrect->setText(QCoreApplication::translate("MainWindow", "Scatter Correct", nullptr));
        btnAdvanced->setText(QCoreApplication::translate("MainWindow", "Advanced Mode", nullptr));
        labelImageRaw->setText(QString());
        label_Id->setText(QCoreApplication::translate("MainWindow", "ID:xxxxxx-xxxx", nullptr));
        cbShowOrgans->setText(QCoreApplication::translate("MainWindow", "Show Organs", nullptr));
        btnInfo->setText(QCoreApplication::translate("MainWindow", "i", nullptr));
        btnExit->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
