#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QDialog>

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
};

#endif // PROGRESSBAR_H
