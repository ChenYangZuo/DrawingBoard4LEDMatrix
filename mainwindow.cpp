#include "mainwindow.h"
#include "./ui_mainwindow.h"


#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);


    for(int i=0; i<10; i++) {
        colorWidget *mcolorWidget = new colorWidget(ui->groupBox_color);
        colorWidgetList_.append(mcolorWidget);
        ui->gridLayout_2->addWidget(mcolorWidget, i/2, i%2, 1, 1);
        connect(mcolorWidget, SIGNAL(colorSignal(QColor)), this->ui->drawingwidget, SLOT(color_changed(QColor)));
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_action_save_triggered() {
    ui->drawingwidget->print();
}

void MainWindow::on_action_import_triggered() {
    ui->drawingwidget->import();
}

