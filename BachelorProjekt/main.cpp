#include "scui.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Scui w;
    w.showMaximized();
    return a.exec();
}
