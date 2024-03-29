#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QDialog>
#include "scui.h"

namespace Ui {
class InformationWindow;
}

class InformationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InformationWindow(Scui *parent);
    ~InformationWindow();

private:
    Ui::InformationWindow *ui;
};

#endif // INFORMATIONWINDOW_H
