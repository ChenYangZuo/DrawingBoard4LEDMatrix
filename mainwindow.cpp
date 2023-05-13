#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QList<QColor> colorList = {QColor(0, 161, 233), QColor(229, 0, 127), QColor(86, 116, 114), QColor(239, 132, 54), QColor(245, 182, 209), QColor(68, 138, 125), QColor(159, 217, 247), QColor(0, 154, 68), QColor(234, 82, 105), QColor(255, 255, 255)};
    for(int i=0; i<10; i++) {
        colorWidget *mcolorWidget = new colorWidget(ui->groupBox_color, colorList[i]);
        ui->gridLayout_2->addWidget(mcolorWidget, i/2, i%2, 1, 1);
        connect(mcolorWidget, SIGNAL(colorSignal(QColor)), this->ui->drawingwidget, SLOT(color_changed(QColor)));
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_action_save_triggered() {
    ui->drawingwidget->exportDrawingBoard();
}

void MainWindow::on_action_import_triggered() {

}


void MainWindow::on_action_clc_triggered() {
    ui->drawingwidget->resetDrawingBoard();
}


void MainWindow::on_action_open_triggered() {
    ui->drawingwidget->importDrawingBoard();
}

