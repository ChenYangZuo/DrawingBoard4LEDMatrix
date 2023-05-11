#include "drawingwidget.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFileDialog>
#include <QCoreApplication>
#include <QMessageBox>

DrawingWidget::DrawingWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(513, 513);
    resetDrawingBoard();
    currentColor_ = QColor(0,0,0);
}

void DrawingWidget::exportDrawingBoard() {
//    qDebug() << colorMap_;
    QJsonArray colorArray;
    for (const QColor& color : qAsConst(colorMap_)) {
        QJsonObject colorObject;
        colorObject.insert("red", color.red());
        colorObject.insert("green", color.green());
        colorObject.insert("blue", color.blue());
        colorArray.append(colorObject);
    }
    QJsonDocument doc(colorArray);
    QString json = doc.toJson(QJsonDocument::Compact);

    QString url = QFileDialog::getSaveFileName(nullptr, "保存画板", "./", "Json文件(*.json)");
    if(url.isNull()){
        return;
    }

    QFile file(url);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this,"错误","文件保存出错");
        return;
    }

    QTextStream out(&file);

    out << json;
    file.close();
}

QList<QColor> DrawingWidget::importDrawingBoard() {
    QString url = QFileDialog::getOpenFileName(nullptr, "打开画板", "./", "Json文件(*.json)");
    if(url.isNull()){
        return QList<QColor>();
    }

    QFile file(url);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,"错误","文件保存出错");
        return QList<QColor>();
    }

    QTextStream in(&file);
    QString json;
    in >> json;
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonArray colorArray = doc.array();
    QList<QColor> colorList;
    for (const QJsonValue& value : colorArray) {
        QJsonObject colorObject = value.toObject();
        int red = colorObject.value("red").toInt();
        int green = colorObject.value("green").toInt();
        int blue = colorObject.value("blue").toInt();
        colorList.append(QColor(red, green, blue));
    }

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
    currentColor_ = QColor(0,0,0);
}

QPixmap DrawingWidget::getPixmap() const {
    QPixmap pixmap = QPixmap::fromImage(image_.scaled(16, 16));
    pixmap.setDevicePixelRatio(devicePixelRatioF());
    return pixmap;
}

void DrawingWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawPixel(event->pos());
    }
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPos_ = QPoint();
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        drawLine(lastPos_, event->pos());
    }
    else {
//        fillGrid();
//        highlightCell(event->pos());
    }
}

void DrawingWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(rect(), image_);
}

void DrawingWidget::enterEvent(QEvent *) {
    setMouseTracking(true);
}

void DrawingWidget::leaveEvent(QEvent *) {
    setMouseTracking(false);
//    fillGrid();
}

void DrawingWidget::fillGrid() {
    image_.fill(Qt::white);
    QPainter painter(&image_);
    QPen pen(Qt::DashLine);
    pen.setWidth(1);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            painter.drawRect(i * 32, j * 32, 32, 32);
        }
    }
}

void DrawingWidget::highlightCell(const QPoint &pos) {
    QPainter painter(&image_);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::lightGray));
    int cellSize = width() / 16;
    int row = pos.y() / cellSize;
    int col = pos.x() / cellSize;
    painter.drawRect(col * cellSize, row * cellSize, cellSize, cellSize);
    update();
}

void DrawingWidget::drawPixel(const QPoint &pos) {
    QPainter painter(&image_);
    painter.setPen(Qt::black);
    int cellSize = width() / 16;
    int row = pos.y() / cellSize;
    int col = pos.x() / cellSize;
    painter.fillRect(col * cellSize, row * cellSize, cellSize, cellSize, currentColor_);

    colorMap_.replace(row * 16 + col, currentColor_);

    lastPos_ = pos;
    update();
}

void DrawingWidget::drawPixel(int row, int col, QColor color) {
    QPainter painter(&image_);
    painter.setPen(Qt::black);
    int cellSize = width() / 16;
    painter.fillRect(col * cellSize, row * cellSize, cellSize, cellSize, color);

    update();
}

void DrawingWidget::drawLine(const QPoint &from, const QPoint &to) {
    QPainter painter(&image_);
    painter.setPen(Qt::black);
    int cellSize = width() / 16;
    int row1 = from.y() / cellSize;
    int col1 = from.x() / cellSize;
    int row2 = to.y() / cellSize;
    int col2 = to.x() / cellSize;
    int dx = abs(col2 - col1);
    int dy = abs(row2 - row1);
    int sx = col1 < col2 ? 1 : -1;
    int sy = row1 < row2 ? 1 : -1;
    int err = dx - dy;

    while (true) {
        painter.fillRect(col1 * cellSize, row1 * cellSize, cellSize, cellSize, currentColor_);
        colorMap_.replace(row1 * 16 + col1, currentColor_);

        if (col1 == col2 && row1 == row2) {
            break;
        }

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            col1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            row1 += sy;
        }
    }

    lastPos_ = to;
    update();
}

void DrawingWidget::color_changed(QColor newColor) {
    qDebug() << newColor;
    this->currentColor_ = newColor;
}
