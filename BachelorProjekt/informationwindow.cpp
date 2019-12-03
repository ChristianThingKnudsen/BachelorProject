#include "informationwindow.h"
#include "ui_informationwindow.h"

InformationWindow::InformationWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformationWindow)
{
    ui->setupUi(this);
    //Setting title and icon
    this->setWindowTitle("Information Window");
    //this->setWindowIcon(QIcon(parent->Root+QString("/pictures/logo.png")));
    QPixmap pix("/Users/ct-10/OneDrive - Aarhus universitet/7 Semester ST/Bachelor/UI_Kode/BachelorProject/pictures/scui.png");
    pix.scaled(660,360);
    ui->labelImage->setPixmap(pix);
}

InformationWindow::~InformationWindow()
{
    delete ui;
}
