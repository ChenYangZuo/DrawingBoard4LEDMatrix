#include "colorwidget.h"
#include <QDebug>
#include <QColorDialog>
#include <QPainter>

colorWidget::colorWidget(QWidget *parent, QColor color, bool checked) : QWidget{parent} {
    this->color_ = color;
    this->checked = checked;
    setFixedSize(24, 24);
}

void colorWidget::setChecked(bool checked) {
//    qDebug() << ">>" << checked;
    if(this->checked != checked){
        this->checked = checked;
        update();
    }
}


void colorWidget::mouseReleaseEvent(QMouseEvent *event) {
    emit change();
    if (event->button() == Qt::LeftButton) {
        emit colorSignal(this->color_);
    }
    else if(event->button() == Qt::RightButton) {
        const QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
        if (color.isValid()) {
            this->color_ = color;
            update();
            emit colorSignal(this->color_);
        }
    }
}

void colorWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), this->color_);
    if(checked){
        painter.setPen(QPen(Qt::black, 4));
        painter.drawRect(rect());
    }
}

QSize colorWidget::sizeHint() {
    int side = qMin(width(), height());
    return QSize(side, side);
}

