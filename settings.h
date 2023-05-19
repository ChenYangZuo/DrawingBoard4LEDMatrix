#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr, QString IP = "127.0.0.1", int PORT = 5555);
    ~Settings();
    QString getIP();
    int getPORT();

private:
    Ui::Settings *ui;
    QString _ip = "";
    int _port = 0;
};

#endif // SETTINGS_H
