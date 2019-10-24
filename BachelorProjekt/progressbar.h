#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QDialog>
//#include "mainwindow.h"
//#include "progressbar.ui"
//#include "ui_progressbar.h"

//class Progressbar;
//class MainWindow;
//class QString;
namespace Ui {
    class Progressbar;
}

class Progressbar : public QDialog
{
    Q_OBJECT

public:
    explicit Progressbar(QWidget *parent = nullptr);
    ~Progressbar();

private:
    Ui::Progressbar *ui;

public slots:
        void setPBValue(int value);
        void setPBText(QString text);
};
#endif // PROGRESSBAR_H

/*
class Progressbar: public QDialog, public Ui::ProgressbarClass {
    Q_OBJECT

public:
    MainWindow *m_pParent{};
    Progressbar();
    explicit Progressbar(MainWindow *parent);

   // explicit Progressbar(QWidget *parent = nullptr);
    //~Progressbar();

//private:
//    Ui::Progressbar *ui;

public slots:
    void setPBValue(int value);
    void setPBText(QString text);

public:
  Ui::ProgressbarClass ui{};
};

#endif // PROGRESSBAR_H
*/

