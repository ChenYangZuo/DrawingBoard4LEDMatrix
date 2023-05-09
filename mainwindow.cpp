#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
//    setCentralWidget(&drawingWidget_);
    // 设置UI外观
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_min_clicked() {
    this->showMinimized();
}


void MainWindow::on_pushButton_max_clicked() {
    if(!this->isMaximized()){
        this->showMaximized();
    }
    else {
        this->showNormal();
    }
}


void MainWindow::on_pushButton_close_clicked() {
    this->close();
}

