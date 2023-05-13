#include "colorwidget.h"
#include <QDebug>
#include <QColorDialog>
#include <QPainter>

colorWidget::colorWidget(QWidget *parent, QColor color) : QWidget{parent} {
    this->color_ = color;
    setFixedSize(24, 24);
}


void colorWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit colorSignal(this->color_);
    }
    else if(event->button() == Qt::RightButton) {
        const QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
        if (color.isValid()) {
            this->color_ = color;
            emit colorSignal(this->color_);
        }
    }
}

void colorWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), this->color_);
}

QSize colorWidget::sizeHint() {
    int side = qMin(width(), height());
    return QSize(side, side);
}

