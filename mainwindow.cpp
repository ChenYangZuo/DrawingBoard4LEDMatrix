#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "settings.h"
#include "socketmanager.h"

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QList<QColor> colorList = {QColor(0, 161, 233), QColor(229, 0, 127), QColor(86, 116, 114), QColor(239, 132, 54), QColor(245, 182, 209), QColor(68, 138, 125), QColor(159, 217, 247), QColor(0, 154, 68), QColor(234, 82, 105), QColor(255, 255, 255)};
    pColorGroup = new ColorGroup(this);

    for(int i=0; i<10; i++) {
        colorWidget *mcolorWidget = new colorWidget(ui->groupBox_color, colorList[i], i==0);
        ui->gridLayout_2->addWidget(mcolorWidget, i/2, i%2, 1, 1);
        pColorGroup->addColorWidget(mcolorWidget);
        connect(mcolorWidget, SIGNAL(colorSignal(QColor)), this->ui->drawingwidget, SLOT(color_changed(QColor)));
    }
    this->ui->drawingwidget->changeColor(colorList[0]);

    this->pButtonGroup = new QButtonGroup();
    pButtonGroup->setExclusive(true);
    pButtonGroup->addButton(this->ui->toolButton_pen);
    pButtonGroup->addButton(this->ui->toolButton_rubber);
    pButtonGroup->addButton(this->ui->toolButton_ink);
    pButtonGroup->addButton(this->ui->toolButton_knife);

    connect(this->ui->toolButton_pen, SIGNAL(clicked()), this->ui->drawingwidget, SLOT(tool_changed()));
    connect(this->ui->toolButton_rubber, SIGNAL(clicked()), this->ui->drawingwidget, SLOT(tool_changed()));
    connect(this->ui->toolButton_ink, SIGNAL(clicked()), this->ui->drawingwidget, SLOT(tool_changed()));
    connect(this->ui->toolButton_knife, SIGNAL(clicked()), this->ui->drawingwidget, SLOT(tool_changed()));

    SocketManager *socketmanager = new SocketManager(this);
    socketmanager->setAddress("192.168.31.202");
    socketmanager->setPort(5555);
    socketmanager->createListener();

    connect(this->ui->drawingwidget, &DrawingWidget::dataReady, socketmanager, &SocketManager::sendMessage);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_action_save_triggered() {
    ui->drawingwidget->exportDrawingBoard();
}

void MainWindow::on_action_clc_triggered() {
    ui->drawingwidget->resetDrawingBoard();
}

void MainWindow::on_action_open_triggered() {
    ui->drawingwidget->importDrawingBoard();
}

void MainWindow::on_action_import_triggered() {

}

void MainWindow::on_action_about_triggered() {
    QMessageBox::information(this, "About", "杭州电子科技大学 左晨洋");
}

void MainWindow::on_action_generate_triggered() {
    ui->drawingwidget->generateCode();
    ui->drawingwidget->sendDrawingBoard();
}

void MainWindow::on_action_settings_triggered() {
    Settings *dialog = new Settings();
    int rtn = dialog->exec();
    if(rtn == QDialog::Accepted){
        QString ip = dialog->getIP();
        int port = dialog->getPORT();
        qDebug() << ip << port;
    }
    delete dialog;
}

