
#ifndef COLORWIDGET_H
#define COLORWIDGET_H


#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>


class colorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit colorWidget(QWidget *parent = nullptr, QColor color = QColor(0,0,0));

private:
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    QSize sizeHint();
    QColor color_ = QColor(0,0,0);

signals:
    void colorSignal(QColor color);

};

#endif // COLORWIDGET_H
