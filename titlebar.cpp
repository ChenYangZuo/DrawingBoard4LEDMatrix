#include "titlebar.h"
#include <QMouseEvent>
#include <QDebug>

TitleBar::TitleBar(QFrame *parent) {
    this->parent = parent;
}

void TitleBar::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::LeftButton && !this->parent->parentWidget()->parentWidget()->isMaximized()){
        this->parent->parentWidget()->parentWidget()->move(event->globalPos() - movePoint);
        event->accept();
    }
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
    movePoint = event->globalPos() - this->parent->parentWidget()->parentWidget()->pos();

    if(drop) {
        if(event->type() == QEvent::MouseButtonDblClick && event->button() == Qt::LeftButton){
            if(!this->parent->parentWidget()->parentWidget()->isMaximized()){
//                this->parent->parentWidget()->setStyleSheet("background-color: rgb(245,247,255);border-radius: 0px;");
                this->parent->parentWidget()->parentWidget()->showMaximized();
            }
            else{
//                this->parent->parentWidget()->setStyleSheet("background-color: rgb(245,247,255);border-radius: 10px;");
                this->parent->parentWidget()->parentWidget()->showNormal();
            }
        }
    }

}

