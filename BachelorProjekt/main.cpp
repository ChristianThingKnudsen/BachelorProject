#include "scui.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Scui w;
    w.setWindowState(Qt::WindowFullScreen); //This will enable full screen at start up
    w.showMaximized();
    return a.exec();
}
