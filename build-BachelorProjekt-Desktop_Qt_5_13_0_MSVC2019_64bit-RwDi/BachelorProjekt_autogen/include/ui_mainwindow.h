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
#include <QtWidgets/QSlider>
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
    QHBoxLayout *horizontalLayoutID;
    QLabel *label_Id;
    QCheckBox *cbShowOrgans;
    QPushButton *btnInfo;
    QPushButton *btnExit;
    QVBoxLayout *verticalLayout_4;
    QFrame *frame1;
    QVBoxLayout *verticalLayout;
    QFrame *frame2;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelRegion;
    QComboBox *comboBox_region;
    QFrame *frame3;
    QHBoxLayout *horizontalLayoutLoadCT;
    QPushButton *btnLoadData;
    QFrame *frame4;
    QHBoxLayout *horizontalLayoutLoadCBCT;
    QPushButton *btnScatterCorrect;
    QFrame *frame5;
    QHBoxLayout *horizontalLayoutSC;
    QPushButton *btnAdvanced;
    QFrame *frame6;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_3;
    qyklabel *labelImageCor;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    qyklabel *labelImageRaw;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QPushButton *btnPlus;
    QSlider *verticalSlider;
    QPushButton *btnMinus;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1085, 526);
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
        frame->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutID = new QHBoxLayout(frame);
        horizontalLayoutID->setObjectName(QString::fromUtf8("horizontalLayoutID"));
        horizontalLayoutID->setContentsMargins(9, -1, -1, -1);
        label_Id = new QLabel(frame);
        label_Id->setObjectName(QString::fromUtf8("label_Id"));
        label_Id->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 20px;\n"
"	background-color: rgba(0,0,0,0%)\n"
"}"));

        horizontalLayoutID->addWidget(label_Id);

        cbShowOrgans = new QCheckBox(frame);
        cbShowOrgans->setObjectName(QString::fromUtf8("cbShowOrgans"));
        cbShowOrgans->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	font-size: 20px;\n"
"	color: #ffffff;\n"
"}"));

        horizontalLayoutID->addWidget(cbShowOrgans);

        btnInfo = new QPushButton(frame);
        btnInfo->setObjectName(QString::fromUtf8("btnInfo"));
        btnInfo->setMinimumSize(QSize(40, 40));
        btnInfo->setMaximumSize(QSize(40, 40));
        btnInfo->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgba(255,190,56,100%); \n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"	border-radius: 20px;\n"
"	border-width: 1.4px;\n"
"	border-color: #000000;\n"
"	border-style: solid;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color:#1367AB;\n"
"}"));

        horizontalLayoutID->addWidget(btnInfo);

        btnExit = new QPushButton(frame);
        btnExit->setObjectName(QString::fromUtf8("btnExit"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(btnExit->sizePolicy().hasHeightForWidth());
        btnExit->setSizePolicy(sizePolicy1);
        btnExit->setMinimumSize(QSize(40, 40));
        btnExit->setMaximumSize(QSize(40, 40));
        btnExit->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgba(255,105,97,100%);\n"
"	color: #ffffff;\n"
"	font-size: 20px;\n"
"	border-width: 1.4px;\n"
"	border-color: #000000;\n"
"	border-style: solid;\n"
"	border-radius: 7px;\n"
"}"));

        horizontalLayoutID->addWidget(btnExit);


        gridLayout_2->addWidget(frame, 0, 1, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));

        gridLayout_2->addLayout(verticalLayout_4, 3, 2, 1, 1);

        frame1 = new QFrame(centralwidget);
        frame1->setObjectName(QString::fromUtf8("frame1"));
        sizePolicy.setHeightForWidth(frame1->sizePolicy().hasHeightForWidth());
        frame1->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(frame1);
        verticalLayout->setSpacing(5);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(3, 0, 1, 0);
        frame2 = new QFrame(frame1);
        frame2->setObjectName(QString::fromUtf8("frame2"));
        frame2->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"	color: #ffffff;\n"
"	\n"
"}"));
        frame2->setLineWidth(1);
        verticalLayout_2 = new QVBoxLayout(frame2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(9, 1, -1, -1);
        labelRegion = new QLabel(frame2);
        labelRegion->setObjectName(QString::fromUtf8("labelRegion"));
        sizePolicy.setHeightForWidth(labelRegion->sizePolicy().hasHeightForWidth());
        labelRegion->setSizePolicy(sizePolicy);
        labelRegion->setMaximumSize(QSize(75, 30));
        labelRegion->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 18px;\n"
"	color:#ffffff;\n"
"	background-color: rgba(0,0,0,0%)\n"
"}"));
        labelRegion->setWordWrap(true);

        verticalLayout_2->addWidget(labelRegion);

        comboBox_region = new QComboBox(frame2);
        comboBox_region->setObjectName(QString::fromUtf8("comboBox_region"));
        comboBox_region->setMaximumSize(QSize(150, 16777215));
        comboBox_region->setStyleSheet(QString::fromUtf8("QComboBox{\n"
"	font-weight: bold;\n"
"	font-size: 18px;\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"	color: #ffffff;\n"
"	border-width: 1.4px;\n"
"	border-color: #000000;\n"
"	border-style: solid;\n"
"	border-radius: 7px;\n"
"	\n"
"}\n"
"QComboBox QAbstractItemView{\n"
"	selection-background-color: rgba(255,190,56,100%); \n"
"}\n"
"\n"
"QComboBox::drop-down{\n"
"	border: 0px;\n"
"}\n"
"\n"
"QComboBox::down-arrow {\n"
"	image: url(':/pictures/dropdownarrow.png');\n"
"    width: 14px;\n"
"    height: 14px;\n"
"}\n"
"\n"
"\n"
"\n"
"\n"
""));

        verticalLayout_2->addWidget(comboBox_region);


        verticalLayout->addWidget(frame2);

        frame3 = new QFrame(frame1);
        frame3->setObjectName(QString::fromUtf8("frame3"));
        frame3->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutLoadCT = new QHBoxLayout(frame3);
        horizontalLayoutLoadCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCT"));
        btnLoadData = new QPushButton(frame3);
        btnLoadData->setObjectName(QString::fromUtf8("btnLoadData"));
        btnLoadData->setMaximumSize(QSize(150, 16777215));
        btnLoadData->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 18px;\n"
"	border-width: 1.4px;\n"
"	border-color: #000000;\n"
"	border-style: solid;\n"
"	border-radius: 7px;\n"
"	\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCT->addWidget(btnLoadData);


        verticalLayout->addWidget(frame3);

        frame4 = new QFrame(frame1);
        frame4->setObjectName(QString::fromUtf8("frame4"));
        frame4->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutLoadCBCT = new QHBoxLayout(frame4);
        horizontalLayoutLoadCBCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCBCT"));
        btnScatterCorrect = new QPushButton(frame4);
        btnScatterCorrect->setObjectName(QString::fromUtf8("btnScatterCorrect"));
        btnScatterCorrect->setEnabled(false);
        btnScatterCorrect->setMaximumSize(QSize(150, 16777215));
        btnScatterCorrect->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgba(19,103,171,60%);\n"
"	color: rgba(255,255,255,60%);\n"
"	font-size: 18px;\n"
"	border-width: 1.4px;\n"
"	border-color: rgba(0,0,0,60%);\n"
"	border-style: solid;\n"
"	border-radius: 7px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCBCT->addWidget(btnScatterCorrect);


        verticalLayout->addWidget(frame4);

        frame5 = new QFrame(frame1);
        frame5->setObjectName(QString::fromUtf8("frame5"));
        frame5->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"	color: #ffffff;\n"
"}"));
        horizontalLayoutSC = new QHBoxLayout(frame5);
        horizontalLayoutSC->setObjectName(QString::fromUtf8("horizontalLayoutSC"));
        horizontalLayoutSC->setContentsMargins(-1, -1, -1, 9);
        btnAdvanced = new QPushButton(frame5);
        btnAdvanced->setObjectName(QString::fromUtf8("btnAdvanced"));
        btnAdvanced->setMaximumSize(QSize(150, 16777215));
        btnAdvanced->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 18px;\n"
"	border-width: 1.4px;\n"
"	border-color: #000000;\n"
"	border-style: solid;\n"
"	border-radius: 7px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutSC->addWidget(btnAdvanced);


        verticalLayout->addWidget(frame5);


        gridLayout_2->addWidget(frame1, 0, 0, 2, 1);

        frame6 = new QFrame(centralwidget);
        frame6->setObjectName(QString::fromUtf8("frame6"));
        frame6->setStyleSheet(QString::fromUtf8("QFrame{\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"	color: #ffffff;\n"
"}"));
        gridLayout = new QGridLayout(frame6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setContentsMargins(9, 9, -1, -1);
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        label_3 = new QLabel(frame6);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        verticalLayout_5->addWidget(label_3, 0, Qt::AlignHCenter);

        labelImageCor = new qyklabel(frame6);
        labelImageCor->setObjectName(QString::fromUtf8("labelImageCor"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(labelImageCor->sizePolicy().hasHeightForWidth());
        labelImageCor->setSizePolicy(sizePolicy2);
        labelImageCor->setMinimumSize(QSize(256, 256));
        labelImageCor->setMaximumSize(QSize(1024, 1024));
        labelImageCor->setMouseTracking(true);
        labelImageCor->setFrameShape(QFrame::Box);

        verticalLayout_5->addWidget(labelImageCor);


        gridLayout->addLayout(verticalLayout_5, 0, 2, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        label_2 = new QLabel(frame6);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_3->addWidget(label_2, 0, Qt::AlignHCenter);

        labelImageRaw = new qyklabel(frame6);
        labelImageRaw->setObjectName(QString::fromUtf8("labelImageRaw"));
        sizePolicy2.setHeightForWidth(labelImageRaw->sizePolicy().hasHeightForWidth());
        labelImageRaw->setSizePolicy(sizePolicy2);
        labelImageRaw->setMinimumSize(QSize(256, 256));
        labelImageRaw->setMaximumSize(QSize(1024, 1024));
        labelImageRaw->setMouseTracking(true);
        labelImageRaw->setFrameShape(QFrame::Box);

        verticalLayout_3->addWidget(labelImageRaw);


        gridLayout->addLayout(verticalLayout_3, 0, 0, 1, 1);

        frame_2 = new QFrame(frame6);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setStyleSheet(QString::fromUtf8(""));
        verticalLayout_6 = new QVBoxLayout(frame_2);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, -1);
        label = new QLabel(frame_2);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label, 0, Qt::AlignHCenter);


        verticalLayout_6->addLayout(horizontalLayout);

        btnPlus = new QPushButton(frame_2);
        btnPlus->setObjectName(QString::fromUtf8("btnPlus"));
        btnPlus->setMinimumSize(QSize(25, 0));
        btnPlus->setMaximumSize(QSize(25, 16777215));
        btnPlus->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 18px;\n"
"	border-width: 1.4px;\n"
"	border-color: #000000;\n"
"	border-style: solid;\n"
"	border-radius: 7px;\n"
"	width: 13 px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        verticalLayout_6->addWidget(btnPlus, 0, Qt::AlignHCenter);

        verticalSlider = new QSlider(frame_2);
        verticalSlider->setObjectName(QString::fromUtf8("verticalSlider"));
        verticalSlider->setStyleSheet(QString::fromUtf8("handle:horizontal {color: red}"));
        verticalSlider->setMaximum(200);
        verticalSlider->setValue(99);
        verticalSlider->setOrientation(Qt::Vertical);
        verticalSlider->setTickPosition(QSlider::TicksBothSides);

        verticalLayout_6->addWidget(verticalSlider, 0, Qt::AlignHCenter);

        btnMinus = new QPushButton(frame_2);
        btnMinus->setObjectName(QString::fromUtf8("btnMinus"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(btnMinus->sizePolicy().hasHeightForWidth());
        btnMinus->setSizePolicy(sizePolicy3);
        btnMinus->setMinimumSize(QSize(25, 0));
        btnMinus->setMaximumSize(QSize(23, 16777215));
        btnMinus->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"	font-size: 18px;\n"
"	border-width: 1.4px;\n"
"	border-color: #000000;\n"
"	border-style: solid;\n"
"	border-radius: 7px;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        verticalLayout_6->addWidget(btnMinus, 0, Qt::AlignHCenter);


        gridLayout->addWidget(frame_2, 0, 8, 1, 1);


        gridLayout_2->addWidget(frame6, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);
        QObject::connect(btnScatterCorrect, SIGNAL(pressed()), MainWindow, SLOT(SLT_DoRegistrationRigid()));
        QObject::connect(btnAdvanced, SIGNAL(pressed()), MainWindow, SLOT(SLT_OpenAdvancedMode()));
        QObject::connect(btnLoadData, SIGNAL(clicked()), MainWindow, SLOT(SLT_SetHisDir()));
        QObject::connect(btnInfo, SIGNAL(pressed()), MainWindow, SLOT(SLT_OpenInfo()));
        QObject::connect(btnExit, SIGNAL(pressed()), MainWindow, SLOT(SLT_Exit()));
        QObject::connect(btnPlus, SIGNAL(clicked()), MainWindow, SLOT(SLT_IncreaseSliderValue()));
        QObject::connect(btnMinus, SIGNAL(clicked()), MainWindow, SLOT(SLT_DecreaseSliderValue()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SCUI", nullptr));
        label_Id->setText(QCoreApplication::translate("MainWindow", "ID:xxxxxx-xxxx", nullptr));
        cbShowOrgans->setText(QCoreApplication::translate("MainWindow", "Show Organs", nullptr));
        btnInfo->setText(QString());
        btnExit->setText(QString());
        labelRegion->setText(QCoreApplication::translate("MainWindow", "Region:", nullptr));
        btnLoadData->setText(QCoreApplication::translate("MainWindow", "Load Data", nullptr));
        btnScatterCorrect->setText(QCoreApplication::translate("MainWindow", "Scatter Correct", nullptr));
        btnAdvanced->setText(QCoreApplication::translate("MainWindow", "Advanced Mode", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Cor image", nullptr));
        labelImageCor->setText(QString());
        label_2->setText(QCoreApplication::translate("MainWindow", "Raw image", nullptr));
        labelImageRaw->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Slice: 100 ", nullptr));
        btnPlus->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        btnMinus->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
