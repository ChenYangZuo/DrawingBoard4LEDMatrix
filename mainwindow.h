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

private:
    Ui::MainWindow *ui;
    QList<colorWidget*> colorWidgetList_;


};
#endif // MAINWINDOW_H
