#include "scui.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Scui w;
    a.setWindowIcon(QIcon(QString("/Users/ct-10/Desktop/logo.png")));
    //w.setWindowState(Qt::WindowFullScreen); //This will enable full screen at start up
    w.showMaximized();
    //w.show();
    return a.exec();
}
