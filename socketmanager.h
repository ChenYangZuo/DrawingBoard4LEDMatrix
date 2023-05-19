#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

class SocketManager : public QObject {
    Q_OBJECT
public:
    explicit SocketManager(QObject *parent = nullptr);

    QString getAddress();

    int getPort();

    void setAddress(QString address);

    void setPort(int port);

    void createListener();

public slots:
    void sendMessage(const QByteArray *data);

private:
    QString address = "127.0.0.1";
    int port = 5555;
    QTcpServer *server;
    QTcpSocket *client;

private slots:
    void on_newConnection_slot();

    void on_readyRead_solt();
};

#endif // SOCKETMANAGER_H
