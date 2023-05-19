#include "socketmanager.h"

SocketManager::SocketManager(QObject *parent) : QObject(parent)
{

}

QString SocketManager::getAddress()
{
    return this->address;
}

int SocketManager::getPort()
{
    return this->port;
}

void SocketManager::setAddress(QString address)
{
    this->address = address;
}

void SocketManager::setPort(int port)
{
    this->port = port;
}

void SocketManager::createListener()
{
    server = new QTcpServer();
    server->listen(QHostAddress(address), port);
    connect(server, &QTcpServer::newConnection, this, &SocketManager::on_newConnection_slot);
}

void SocketManager::sendMessage(const QByteArray *data) {
    if(client != nullptr){
        client->write(*data);
        qDebug() << "Sending Settings";
    }
    else{
        qDebug() << "No Client";
    }
}

void SocketManager::on_newConnection_slot() {
    client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &SocketManager::on_readyRead_solt);
}

void SocketManager::on_readyRead_solt() {
    QByteArray data = client->readAll();
    QString msg = QString::fromLocal8Bit(data);
    qDebug() << msg;
//    QByteArray send_data;
//    send_data.append((uint8_t)0xa5);
//    send_data.append((uint8_t)0x5a);
//    send_data.append((uint8_t)500);
//    client->write(send_data);
//    qDebug() << "Set Time 500ms";
}

