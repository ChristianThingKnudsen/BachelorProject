#include "progressbar.h"
#include "ui_progressbar.h"

Progressbar::Progressbar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Progressbar)
{
    ui->setupUi(this);
}

Progressbar::~Progressbar()
{
    delete ui;
}

void Progressbar::setPBValue(int value){
    ui->progressBar->setValue(value);
}

void Progressbar::setPBText(QString text){
    ui->labelStatus->setText(text);
}
