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
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Progressbar
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *labelStatus;
    QProgressBar *progressBar;

    void setupUi(QDialog *Progressbar)
    {
        if (Progressbar->objectName().isEmpty())
            Progressbar->setObjectName(QString::fromUtf8("Progressbar"));
        Progressbar->resize(400, 241);
        Progressbar->setStyleSheet(QString::fromUtf8("QDialog{\n"
"	background-color: #1367AB;\n"
"}"));
        verticalLayoutWidget = new QWidget(Progressbar);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(9, 9, 381, 221));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        labelStatus = new QLabel(verticalLayoutWidget);
        labelStatus->setObjectName(QString::fromUtf8("labelStatus"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelStatus->sizePolicy().hasHeightForWidth());
        labelStatus->setSizePolicy(sizePolicy);
        labelStatus->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	font-size: 20px;\n"
"	color: #ffffff;\n"
"}"));

        verticalLayout->addWidget(labelStatus);

        progressBar = new QProgressBar(verticalLayoutWidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);


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
