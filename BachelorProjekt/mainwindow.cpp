#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qyklabel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("Head-Neck");
    ui->comboBox->addItem("Prostata");
    ui->comboBox->addItem("Thorax");
}

MainWindow::~MainWindow()
{
    delete ui;
}

