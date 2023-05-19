#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent, QString IP, int PORT) : QDialog(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->_ip = IP;
    this->_port = PORT;
    this->ui->lineEdit_IP->setText(this->_ip);
    this->ui->lineEdit_PORT->setText(QString("%1").arg(this->_port));
}

Settings::~Settings() {
    delete ui;
}

QString Settings::getIP() {
    return _ip;
}

int Settings::getPORT() {
    return _port;
}
