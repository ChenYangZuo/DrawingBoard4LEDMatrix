#include "drawingwidget.h"


DrawingWidget::DrawingWidget(QWidget *parent) : QWidget(parent) {
    setFixedSize(513, 513);
    fillGrid();
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
    painter.fillRect(col * cellSize, row * cellSize, cellSize, cellSize, Qt::black);
    lastPos_ = pos;
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
        painter.fillRect(col1 * cellSize, row1 * cellSize, cellSize, cellSize, Qt::black);

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
