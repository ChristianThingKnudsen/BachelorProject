#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.setWindowState(Qt::WindowFullScreen); //This will enable full screen at start up
    w.showMaximized();
    //w.show();
    return a.exec();
}
