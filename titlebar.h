#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QFrame>
#include <QMainWindow>

class TitleBar : public QFrame {

public:
    TitleBar(QFrame *parent=0);
    bool drop = false;

private:
    QFrame *parent;
    QPoint movePoint;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

};

#endif // TITLEBAR_H
