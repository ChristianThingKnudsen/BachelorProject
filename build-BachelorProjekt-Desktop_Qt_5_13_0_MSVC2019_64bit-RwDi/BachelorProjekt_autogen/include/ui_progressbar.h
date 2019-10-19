/********************************************************************************
** Form generated from reading UI file 'progressbar.ui'
**
** Created by: Qt User Interface Compiler version 5.13.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROGRESSBAR_H
#define UI_PROGRESSBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Progressbar
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelStatus;
    QProgressBar *progressBar;

    void setupUi(QDialog *Progressbar)
    {
        if (Progressbar->objectName().isEmpty())
            Progressbar->setObjectName(QString::fromUtf8("Progressbar"));
        Progressbar->setEnabled(true);
        Progressbar->resize(416, 253);
        Progressbar->setStyleSheet(QString::fromUtf8("QDialog{\n"
"	background-color: qradialgradient(spread:reflect, cx:0.5, cy:0.5, radius:0.7, fx:0.499, fy:0.505682, stop:0 rgba(20, 106, 173, 253), stop:0.756757 rgba(20, 69, 109, 255));\n"
"}"));
        gridLayout_2 = new QGridLayout(Progressbar);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetMaximumSize);
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setVerticalSpacing(0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelStatus = new QLabel(Progressbar);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelStatus->sizePolicy().hasHeightForWidth());
        labelStatus->setSizePolicy(sizePolicy);
        labelStatus->setMaximumSize(QSize(16777215, 45));
        labelStatus->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 20px;\n"
"	color: #ffffff;\n"
"}"));

        verticalLayout->addWidget(labelStatus);

        progressBar = new QProgressBar(Progressbar);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setMaximumSize(QSize(16777215, 45));
        progressBar->setStyleSheet(QString::fromUtf8(" QProgressBar::chunk {\n"
"     background-color: rgb(47,212,75);\n"
"     width: 20px;\n"
"	height: 4px;\n"
" }"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(Progressbar);

        QMetaObject::connectSlotsByName(Progressbar);
    } // setupUi

    void retranslateUi(QDialog *Progressbar)
    {
        Progressbar->setWindowTitle(QCoreApplication::translate("Progressbar", "Dialog", nullptr));
        labelStatus->setText(QCoreApplication::translate("Progressbar", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Progressbar: public Ui_Progressbar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROGRESSBAR_H
