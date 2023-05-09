#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "drawingwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_min_clicked();

    void on_pushButton_max_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::MainWindow *ui;

    DrawingWidget drawingWidget_;
};
#endif // MAINWINDOW_H
