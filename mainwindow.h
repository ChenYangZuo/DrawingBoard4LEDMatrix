#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "drawingwidget.h"
#include "colorwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_save_triggered();

    void on_action_import_triggered();

    void on_action_clc_triggered();

    void on_action_open_triggered();

    void on_action_about_triggered();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
