#include "mainwindow.h"

#include <QApplication>

//CBCTRecon headers
/*
#include "cbctrecon_mainwidget.h"
#include "DlgExternalCommand.h"
#include "DlgHistogram.h"
#include "DlgRegistration.h"
#include "qcustomplot.h"
#include "qyklabel.h"
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setWindowState(Qt::WindowFullScreen);
    w.show();
    return a.exec();
}
