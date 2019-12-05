#include "informationwindow.h"
#include "ui_informationwindow.h"
#include "scui.h"

InformationWindow::InformationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformationWindow)
{
    ui->setupUi(this);
    Scui *scui = new Scui;

    //Setting title and icon
    this->setWindowTitle("Information Window");
    this->setWindowIcon(QIcon(scui->Root+QString("/pictures/logo.png")));
    QPixmap pix(scui->Root+QString("/pictures/scui.png"));
    int w = ui->labelImage->width();
    int h = ui->labelImage->height();
    ui->labelImage->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    ui->labelImage->setAlignment(Qt::AlignTop);
}

InformationWindow::~InformationWindow()
{
    delete ui;
}
