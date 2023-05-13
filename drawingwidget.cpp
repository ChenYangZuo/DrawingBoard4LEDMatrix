#include "drawingwidget.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QCoreApplication>
#include <QMessageBox>

DrawingWidget::DrawingWidget(QWidget *parent) : QWidget(parent) {
//    setFixedSize(529, 529);
    resetDrawingBoard();
    currentColor_ = QColor(0,0,0);
}

void DrawingWidget::exportDrawingBoard() {
    QString url = QFileDialog::getSaveFileName(nullptr, "保存画板", "./", "Data文件(*.dat)");
    if(url.isNull()){
        return;
    }
    QFile file(url);
    if (!file.open(QIODevice::WriteOnly)){
        QMessageBox::critical(this,"错误","文件保存出错");
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_0);

    out << colorMap_;
    file.close();
}

QList<QColor> DrawingWidget::importDrawingBoard() {
    QString url = QFileDialog::getOpenFileName(nullptr, "打开画板", "./", "Data文件(*.dat)");
    if(url.isNull()){
        return QList<QColor>();
    }
    QFile file(url);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this,"错误","文件保存出错");
        return QList<QColor>();
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_0);

    QList<QColor> colorList;
    in >> colorList;
    colorMap_ = colorList;

    for(int i=0; i<16; i++){
        for(int j=0; j<16; j++){
            drawPixel(i,j,colorMap_[i*16+j]);
        }
    }

    return colorMap_;
}

void DrawingWidget::resetDrawingBoard() {
    fillGrid();
    colorMap_.clear();
    for(int i=0; i<256; i++){
        colorMap_.append(QColor(255,255,255));
    }
}

void DrawingWidget::mousePressEvent(QMouseEvent *event) {
    QRect rect_ = rect();
    rect_.adjust(0,0,-1,-1);

    if (event->button() == Qt::LeftButton) {
        if(rect_.contains(event->pos())) {
            drawPixel(event->pos());
        }
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
    QRect rect_ = rect();
    rect_.adjust(0,0,-1,-1);

    if (event->buttons() & Qt::LeftButton) {
        if(rect_.contains(event->pos())){
//            qDebug() << event->pos();
            drawPixel(event->pos());
        }
    }
}

void DrawingWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(rect(), image_);
}

void DrawingWidget::fillGrid() {
    int cellSize = 32;

    image_.fill(Qt::white);
    QPainter painter(&image_);
    QPen pen(Qt::DashLine);
    pen.setWidth(1);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    for(int i=0; i<17; i++){
        painter.drawLine(0, i*(cellSize+1), 528, i*(cellSize+1)); //横向
        painter.drawLine(i*(cellSize+1), 0, i*(cellSize+1), 528); //纵向
    }

    update();
}

void DrawingWidget::drawPixel(const QPoint &pos) {
    int cellSize = 32;
    int row = pos.y() / 33;
    int col = pos.x() / 33;

    QPainter painter(&image_);
    painter.setPen(Qt::black);
    painter.fillRect(col * (cellSize+1)+1, row * (cellSize+1)+1, cellSize, cellSize, currentColor_);

    colorMap_.replace(row * 16 + col, currentColor_);

    update();
}

void DrawingWidget::drawPixel(int row, int col, QColor color) {
    int cellSize = 32;

    QPainter painter(&image_);
    painter.setPen(Qt::black);
    painter.fillRect(col * (cellSize+1)+1, row * (cellSize+1)+1, cellSize, cellSize, color);

    update();
}

void DrawingWidget::color_changed(QColor newColor) {
    qDebug() << newColor;
    this->currentColor_ = newColor;
}
