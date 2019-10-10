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
#include <QtWidgets/QStatusBar>
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
    QComboBox *comboBox;
    QFrame *frame2;
    QHBoxLayout *horizontalLayoutLoadCT;
    QPushButton *btnLoadCT;
    QCheckBox *cbCT;
    QFrame *frame3;
    QHBoxLayout *horizontalLayoutLoadCBCT;
    QPushButton *btnLoadCBCT;
    QCheckBox *cbCBCT;
    QFrame *frame4;
    QHBoxLayout *horizontalLayoutSC;
    QPushButton *btnScatterCorrect;
    QFrame *frame5;
    QHBoxLayout *horizontalLayoutID;
    QLabel *label_Id;
    QCheckBox *cbShowOrgans;
    QPushButton *btnInfo;
    QFrame *frame6;
    QGridLayout *gridLayout;
    qyklabel *labelImageRaw;
    QFrame *frame7;
    QVBoxLayout *verticalLayout_2;
    QPushButton *btnAdvanced;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(979, 544);
        MainWindow->setStyleSheet(QString::fromUtf8("MainWindow{\n"
"	background-color: #1367AB;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 0, -1, 0);
        frame1 = new QFrame(frame);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        frame1->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutRegion = new QHBoxLayout(frame1);
        horizontalLayoutRegion->setObjectName(QString::fromUtf8("horizontalLayoutRegion"));
        labelRegion = new QLabel(frame1);
        labelRegion->setObjectName(QString::fromUtf8("labelRegion"));
        labelRegion->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 20px;\n"
"}"));

        horizontalLayoutRegion->addWidget(labelRegion);

        comboBox = new QComboBox(frame1);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"	font-weight: bold;\n"
"	font-size: 15px;\n"
"}"));

        horizontalLayoutRegion->addWidget(comboBox);


        verticalLayout->addWidget(frame1);

        frame2 = new QFrame(frame);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutLoadCT = new QHBoxLayout(frame2);
        horizontalLayoutLoadCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCT"));
        btnLoadCT = new QPushButton(frame2);
        btnLoadCT->setObjectName(QString::fromUtf8("btnLoadCT"));
        btnLoadCT->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCT->addWidget(btnLoadCT);

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
"background-color: #FF0000;\n"
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
        btnLoadCBCT = new QPushButton(frame3);
        btnLoadCBCT->setObjectName(QString::fromUtf8("btnLoadCBCT"));
        btnLoadCBCT->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCBCT->addWidget(btnLoadCBCT);

        cbCBCT = new QCheckBox(frame3);
        cbCBCT->setObjectName(QString::fromUtf8("cbCBCT"));
        cbCBCT->setEnabled(false);
        sizePolicy1.setHeightForWidth(cbCBCT->sizePolicy().hasHeightForWidth());
        cbCBCT->setSizePolicy(sizePolicy1);
        cbCBCT->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	background-color: #FF0000;\n"
"}\n"
"QCheckBox:checked{\n"
"	background-color: #FFFFFF;\n"
"}\n"
""));
        cbCBCT->setCheckable(true);

        horizontalLayoutLoadCBCT->addWidget(cbCBCT);


        verticalLayout->addWidget(frame3);

        frame4 = new QFrame(frame);
        frame4->setObjectName(QString::fromUtf8("frame4"));
        frame4->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutSC = new QHBoxLayout(frame4);
        horizontalLayoutSC->setObjectName(QString::fromUtf8("horizontalLayoutSC"));
        btnScatterCorrect = new QPushButton(frame4);
        btnScatterCorrect->setObjectName(QString::fromUtf8("btnScatterCorrect"));
        btnScatterCorrect->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutSC->addWidget(btnScatterCorrect);


        verticalLayout->addWidget(frame4);


        gridLayout_2->addWidget(frame, 0, 0, 2, 1);

        frame5 = new QFrame(centralwidget);
        frame5->setObjectName(QString::fromUtf8("frame5"));
        frame5->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutID = new QHBoxLayout(frame5);
        horizontalLayoutID->setObjectName(QString::fromUtf8("horizontalLayoutID"));
        label_Id = new QLabel(frame5);
        label_Id->setObjectName(QString::fromUtf8("label_Id"));
        label_Id->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 20px;\n"
"}"));

        horizontalLayoutID->addWidget(label_Id);

        cbShowOrgans = new QCheckBox(frame5);
        cbShowOrgans->setObjectName(QString::fromUtf8("cbShowOrgans"));
        cbShowOrgans->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	font-size: 20px;\n"
"	color: #ffffff;\n"
"}"));

        horizontalLayoutID->addWidget(cbShowOrgans);

        btnInfo = new QPushButton(frame5);
        btnInfo->setObjectName(QString::fromUtf8("btnInfo"));
        btnInfo->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color:#E4A115; \n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color:#1367AB;\n"
"}"));

        horizontalLayoutID->addWidget(btnInfo);


        gridLayout_2->addWidget(frame5, 0, 1, 1, 1);

        frame6 = new QFrame(centralwidget);
        frame6->setObjectName(QString::fromUtf8("frame6"));
        frame6->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        gridLayout = new QGridLayout(frame6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelImageRaw = new qyklabel(frame6);
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


        gridLayout_2->addWidget(frame6, 1, 1, 1, 1);

        frame7 = new QFrame(centralwidget);
        frame7->setObjectName(QString::fromUtf8("frame7"));
        frame7->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: #0D3F64;\n"
"	color: #ffffff;\n"
"}"));
        verticalLayout_2 = new QVBoxLayout(frame7);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        btnAdvanced = new QPushButton(frame7);
        btnAdvanced->setObjectName(QString::fromUtf8("btnAdvanced"));
        btnAdvanced->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        verticalLayout_2->addWidget(btnAdvanced);


        gridLayout_2->addWidget(frame7, 0, 2, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(btnLoadCT, SIGNAL(pressed()), MainWindow, SLOT(SLT_SetHisDir()));
        QObject::connect(btnScatterCorrect, SIGNAL(pressed()), MainWindow, SLOT(SLT_DoRegistrationRigid()));

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
        labelImageRaw->setText(QString());
        btnAdvanced->setText(QCoreApplication::translate("MainWindow", "Advanced Mode", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
