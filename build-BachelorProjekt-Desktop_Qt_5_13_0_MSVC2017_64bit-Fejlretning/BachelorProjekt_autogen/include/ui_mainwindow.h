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
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *SCUI;
    QWidget *verticalLayoutWidget;
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
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayoutImage;
    QLabel *label_ID;
    QLabel *labelImage;
    QCheckBox *cbShowOrgans;
    QWidget *Advanced;
    QMenuBar *menubar;
    QMenu *menuSCUI;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(932, 555);
        MainWindow->setStyleSheet(QString::fromUtf8("MainWindow{\n"
"	background-color: #005194\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 801, 511));
        tabWidget->setStyleSheet(QString::fromUtf8("tabWidget{\n"
"	background-color: #005194\n"
"}"));
        SCUI = new QWidget();
        SCUI->setObjectName(QString::fromUtf8("SCUI"));
        verticalLayoutWidget = new QWidget(SCUI);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 231, 471));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayoutRegion = new QHBoxLayout();
        horizontalLayoutRegion->setObjectName(QString::fromUtf8("horizontalLayoutRegion"));
        labelRegion = new QLabel(verticalLayoutWidget);
        labelRegion->setObjectName(QString::fromUtf8("labelRegion"));

        horizontalLayoutRegion->addWidget(labelRegion);

        comboBox = new QComboBox(verticalLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayoutRegion->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayoutRegion);

        horizontalLayoutLoadCT = new QHBoxLayout();
        horizontalLayoutLoadCT->setObjectName(QString::fromUtf8("horizontalLayoutLoadCT"));
        btnLoadCT = new QPushButton(verticalLayoutWidget);
        btnLoadCT->setObjectName(QString::fromUtf8("btnLoadCT"));
        btnLoadCT->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCT->addWidget(btnLoadCT);

        cbCT = new QCheckBox(verticalLayoutWidget);
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
        btnLoadCBCT = new QPushButton(verticalLayoutWidget);
        btnLoadCBCT->setObjectName(QString::fromUtf8("btnLoadCBCT"));
        btnLoadCBCT->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: #1367AB;\n"
"	color: #ffffff;\n"
"}\n"
"QPushButton:pressed{\n"
"	background-color: #E4A115\n"
"}"));

        horizontalLayoutLoadCBCT->addWidget(btnLoadCBCT);

        cbCBCT = new QCheckBox(verticalLayoutWidget);
        cbCBCT->setObjectName(QString::fromUtf8("cbCBCT"));
        cbCBCT->setStyleSheet(QString::fromUtf8("QCheckBox{\n"
"	check-color: #FF0000;\n"
"}"));

        horizontalLayoutLoadCBCT->addWidget(cbCBCT);


        verticalLayout->addLayout(horizontalLayoutLoadCBCT);

        horizontalLayoutSC = new QHBoxLayout();
        horizontalLayoutSC->setObjectName(QString::fromUtf8("horizontalLayoutSC"));
        btnScatterCorrect = new QPushButton(verticalLayoutWidget);
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

        verticalLayoutWidget_2 = new QWidget(SCUI);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(230, 0, 451, 471));
        verticalLayoutImage = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayoutImage->setObjectName(QString::fromUtf8("verticalLayoutImage"));
        verticalLayoutImage->setContentsMargins(0, 0, 0, 0);
        label_ID = new QLabel(verticalLayoutWidget_2);
        label_ID->setObjectName(QString::fromUtf8("label_ID"));

        verticalLayoutImage->addWidget(label_ID);

        labelImage = new QLabel(verticalLayoutWidget_2);
        labelImage->setObjectName(QString::fromUtf8("labelImage"));
        labelImage->setMargin(10);

        verticalLayoutImage->addWidget(labelImage);

        cbShowOrgans = new QCheckBox(SCUI);
        cbShowOrgans->setObjectName(QString::fromUtf8("cbShowOrgans"));
        cbShowOrgans->setGeometry(QRect(700, 240, 91, 20));
        tabWidget->addTab(SCUI, QString());
        Advanced = new QWidget();
        Advanced->setObjectName(QString::fromUtf8("Advanced"));
        tabWidget->addTab(Advanced, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 932, 21));
        menuSCUI = new QMenu(menubar);
        menuSCUI->setObjectName(QString::fromUtf8("menuSCUI"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSCUI->menuAction());

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


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
        label_ID->setText(QCoreApplication::translate("MainWindow", "ID:xxxxxx-xxxx", nullptr));
        labelImage->setText(QCoreApplication::translate("MainWindow", "This is going to be an image later", nullptr));
        cbShowOrgans->setText(QCoreApplication::translate("MainWindow", "Show Organs", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(SCUI), QCoreApplication::translate("MainWindow", "SCUI", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(Advanced), QCoreApplication::translate("MainWindow", "Advanced", nullptr));
        menuSCUI->setTitle(QCoreApplication::translate("MainWindow", "SCUI", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
