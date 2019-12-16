#include "informationwindow.h"
#include "ui_informationwindow.h"
#include "scui.h"

InformationWindow::InformationWindow(Scui *parent) :
    QDialog(parent),
    ui(new Ui::InformationWindow)
{
    ui->setupUi(this);

    //Setting title and icon
    this->setWindowTitle("Information Window");
    this->setWindowIcon(QIcon(parent->Root+QString("/pictures/logo.png")));
    QPixmap pix(parent->Root+QString("/pictures/scui.png"));
    // Setting image with flow
    int w = ui->labelImage->width(); // Width
    int h = ui->labelImage->height(); // Height
    ui->labelImage->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio)); // Scaling
    ui->labelImage->setAlignment(Qt::AlignTop); // Align top
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);// removes question mark from title bar
}

InformationWindow::~InformationWindow()
{
    delete ui;
}
