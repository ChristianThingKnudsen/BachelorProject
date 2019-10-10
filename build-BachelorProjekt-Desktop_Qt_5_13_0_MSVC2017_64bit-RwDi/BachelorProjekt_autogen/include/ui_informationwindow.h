/********************************************************************************
** Form generated from reading UI file 'informationwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFORMATIONWINDOW_H
#define UI_INFORMATIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InformationWindow
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label_13;
    QLabel *labelRegion;
    QLabel *labelLoadCT;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *labelShowOrgansInfo;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *labelRegionInfo;

    void setupUi(QDialog *InformationWindow)
    {
        if (InformationWindow->objectName().isEmpty())
            InformationWindow->setObjectName(QString::fromUtf8("InformationWindow"));
        InformationWindow->resize(901, 324);
        InformationWindow->setStyleSheet(QString::fromUtf8("QDialog{\n"
"	background-color: #1367AB;\n"
"}"));
        formLayoutWidget = new QWidget(InformationWindow);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(1, 0, 901, 321));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(9);
        formLayout->setVerticalSpacing(15);
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_13 = new QLabel(formLayoutWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_13->sizePolicy().hasHeightForWidth());
        label_13->setSizePolicy(sizePolicy);
        label_13->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_13->setLayoutDirection(Qt::RightToLeft);
        label_13->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	text-decoration: underline;\n"
"	font-size: 20px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"	alignment: center;\n"
"}\n"
""));
        label_13->setFrameShape(QFrame::NoFrame);
        label_13->setTextFormat(Qt::PlainText);

        formLayout->setWidget(1, QFormLayout::SpanningRole, label_13);

        labelRegion = new QLabel(formLayoutWidget);
        labelRegion->setObjectName(QString::fromUtf8("labelRegion"));
        labelRegion->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 15px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"}\n"
""));

        formLayout->setWidget(2, QFormLayout::LabelRole, labelRegion);

        labelLoadCT = new QLabel(formLayoutWidget);
        labelLoadCT->setObjectName(QString::fromUtf8("labelLoadCT"));
        labelLoadCT->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 15px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"}\n"
""));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelLoadCT);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: #ffffff;\n"
"}"));

        formLayout->setWidget(3, QFormLayout::FieldRole, label_2);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 15px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"}\n"
""));

        formLayout->setWidget(4, QFormLayout::LabelRole, label_3);

        label_4 = new QLabel(formLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: #ffffff;\n"
"}"));

        formLayout->setWidget(4, QFormLayout::FieldRole, label_4);

        label_5 = new QLabel(formLayoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 15px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"}\n"
""));

        formLayout->setWidget(5, QFormLayout::LabelRole, label_5);

        label_6 = new QLabel(formLayoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: #ffffff;\n"
"}"));

        formLayout->setWidget(5, QFormLayout::FieldRole, label_6);

        label_7 = new QLabel(formLayoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 15px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"}\n"
""));

        formLayout->setWidget(6, QFormLayout::LabelRole, label_7);

        label_8 = new QLabel(formLayoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: #ffffff;\n"
"}"));

        formLayout->setWidget(6, QFormLayout::FieldRole, label_8);

        label_9 = new QLabel(formLayoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 15px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"}\n"
""));

        formLayout->setWidget(7, QFormLayout::LabelRole, label_9);

        labelShowOrgansInfo = new QLabel(formLayoutWidget);
        labelShowOrgansInfo->setObjectName(QString::fromUtf8("labelShowOrgansInfo"));
        labelShowOrgansInfo->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: #ffffff;\n"
"}"));

        formLayout->setWidget(7, QFormLayout::FieldRole, labelShowOrgansInfo);

        label_11 = new QLabel(formLayoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 15px;\n"
"	color:#E4A115; \n"
"	font-weight: bold;\n"
"}\n"
""));

        formLayout->setWidget(8, QFormLayout::LabelRole, label_11);

        label_12 = new QLabel(formLayoutWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: #ffffff;\n"
"}"));

        formLayout->setWidget(8, QFormLayout::FieldRole, label_12);

        labelRegionInfo = new QLabel(formLayoutWidget);
        labelRegionInfo->setObjectName(QString::fromUtf8("labelRegionInfo"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(labelRegionInfo->sizePolicy().hasHeightForWidth());
        labelRegionInfo->setSizePolicy(sizePolicy1);
        labelRegionInfo->setAutoFillBackground(false);
        labelRegionInfo->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	color: #ffffff;\n"
"}"));

        formLayout->setWidget(2, QFormLayout::FieldRole, labelRegionInfo);


        retranslateUi(InformationWindow);

        QMetaObject::connectSlotsByName(InformationWindow);
    } // setupUi

    void retranslateUi(QDialog *InformationWindow)
    {
        InformationWindow->setWindowTitle(QCoreApplication::translate("InformationWindow", "Dialog", nullptr));
        label_13->setText(QCoreApplication::translate("InformationWindow", "SCUI Information ", nullptr));
        labelRegion->setText(QCoreApplication::translate("InformationWindow", "Region", nullptr));
        labelLoadCT->setText(QCoreApplication::translate("InformationWindow", "Load CT", nullptr));
        label_2->setText(QCoreApplication::translate("InformationWindow", "This button allows the user to read in the taken CT image. The corresponding checkbox will light up green when the image has been loaded. ", nullptr));
        label_3->setText(QCoreApplication::translate("InformationWindow", "Load CBCT", nullptr));
        label_4->setText(QCoreApplication::translate("InformationWindow", "This button allows the user to read in the taken CBCT image. The corresponding checkbox will light up green when the image has been loaded. ", nullptr));
        label_5->setText(QCoreApplication::translate("InformationWindow", "Scatter Correct", nullptr));
        label_6->setText(QCoreApplication::translate("InformationWindow", "This button takes the CT and CBCT image and creates a scatter corrected CBCT image, which can be used for reviewing the dose plan", nullptr));
        label_7->setText(QCoreApplication::translate("InformationWindow", "ID", nullptr));
        label_8->setText(QCoreApplication::translate("InformationWindow", "This label shows the security number of the patient under treatment ", nullptr));
        label_9->setText(QCoreApplication::translate("InformationWindow", "Show Organs", nullptr));
        labelShowOrgansInfo->setText(QCoreApplication::translate("InformationWindow", "When this checkbox is checked the scatter corrected CBCT image, will then be superimposed by the organ image", nullptr));
        label_11->setText(QCoreApplication::translate("InformationWindow", "Advanced mode", nullptr));
        label_12->setText(QCoreApplication::translate("InformationWindow", "This button opens a new form where more values behind the scene can be accessed. This is only recommended for  physicists ", nullptr));
        labelRegionInfo->setText(QCoreApplication::translate("InformationWindow", "This option allows to choose which part of the body that is up for treatment. The program then selects the appropriate values based on the selected region. ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class InformationWindow: public Ui_InformationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFORMATIONWINDOW_H
