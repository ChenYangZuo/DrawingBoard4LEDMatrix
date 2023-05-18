#include "drawingwidget.h"
#include <QDebug>
#include <QFileDialog>
#include <QCoreApplication>
#include <QMessageBox>
#include <QToolButton>

DrawingWidget::DrawingWidget(QWidget *parent, QColor color) : QWidget(parent) {
//    setFixedSize(529, 529);
    resetDrawingBoard();
    currentColor_ = color;
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

    file.close();

    return colorMap_;
}

void DrawingWidget::resetDrawingBoard() {
    fillGrid();
    colorMap_.clear();
    for(int i=0; i<256; i++){
        colorMap_.append(QColor(255,255,255));
    }
}

void DrawingWidget::generateCode() {
    QFile file("./generate.h");
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)){
        return;
    }
    QTextStream out(&file);

    // 灯板的第0个LED在右上方，向左按蛇字形排序
    int index = 0;
    for(int i=0; i<16; i++){
        if(i%2 == 1){
            for(int j=0; j<16; j++){
                out << QString("pixels.setPixelColor(%1, pixels.Color(%2, %3, %4));").arg(index++).arg(colorMap_[i*16+j].red()).arg(colorMap_[i*16+j].green()).arg(colorMap_[i*16+j].blue()) << "\n";
            }
        }
        else{
            for(int j=0; j<16; j++){
                out << QString("pixels.setPixelColor(%1, pixels.Color(%2, %3, %4));").arg(index++).arg(colorMap_[(i+1)*16-(j+1)].red()).arg(colorMap_[(i+1)*16-(j+1)].green()).arg(colorMap_[(i+1)*16-(j+1)].blue()) << "\n";
            }
        }
    }

    file.close();
}

void DrawingWidget::mousePressEvent(QMouseEvent *event) {
    QRect rect_ = rect();
    rect_.adjust(0,0,-1,-1);

    if (event->button() == Qt::LeftButton) {
        if(rect_.contains(event->pos())) {
            switch (currentTool_) {
            case PEN:
                drawPixel(event->pos());
                break;
            case RUBBER:
                erasePixel(event->pos());
                break;
            case KNIFE:
                slakePixel(event->pos());
                break;
            case INK:
                dyePixel(event->pos());
                break;
            default:
                break;
            }
        }
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
    QRect rect_ = rect();
    rect_.adjust(0,0,-1,-1);

    if (event->buttons() & Qt::LeftButton) {
        if(rect_.contains(event->pos())){
            switch (currentTool_) {
            case PEN:
                drawPixel(event->pos());
                break;
            case RUBBER:
                erasePixel(event->pos());
                break;
            case KNIFE:
                slakePixel(event->pos());
                break;
            default:
                break;
            }
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

void DrawingWidget::erasePixel(const QPoint &pos) {
    int cellSize = 32;
    int row = pos.y() / 33;
    int col = pos.x() / 33;

    QPainter painter(&image_);
    painter.setPen(Qt::black);
    painter.fillRect(col * (cellSize+1)+1, row * (cellSize+1)+1, cellSize, cellSize, QColor(255, 255, 255));

    colorMap_.replace(row * 16 + col, QColor(255, 255, 255));

    update();
}

void DrawingWidget::slakePixel(const QPoint &pos) {
    int cellSize = 32;
    int row = pos.y() / 33;
    int col = pos.x() / 33;

    QPainter painter(&image_);
    painter.setPen(Qt::black);
    painter.fillRect(col * (cellSize+1)+1, row * (cellSize+1)+1, cellSize, cellSize, QColor(0, 0, 0));

    colorMap_.replace(row * 16 + col, QColor(0, 0, 0));

    update();
}

void DrawingWidget::dyePixel(const QPoint &pos) {
    int cellSize = 32;
    int row = pos.y() / 33;
    int col = pos.x() / 33;

    QList<int> mask;
    for(int i=0; i<256; i++){
        mask.append(0);
    }

    algorithm_dfs(row, col, colorMap_, mask, colorMap_[row * 16 + col]);

    for(int i=0; i<16; i++) {
        for(int j=0; j<16; j++) {
            if(mask[i*16+j]) {
                drawPixel(i,j,currentColor_);
                colorMap_[i*16+j] = currentColor_;
            }
        }
    }

}

void DrawingWidget::algorithm_dfs(int row, int col, QList<QColor> &map, QList<int> &mask, QColor color) {
    if (row < 0 || row > 15 || col < 0 || col > 15) {
        return;
    }
    if (mask[row * 16 + col] != 0) {
        return;
    }
    if(map[row * 16 + col] != color) {
//        qDebug() << map[row * 16 + col] << currentColor_;
        return;
    }

    mask[row * 16 + col] = 1;

    algorithm_dfs(row+1, col, colorMap_, mask, color);
    algorithm_dfs(row-1, col, colorMap_, mask, color);
    algorithm_dfs(row, col+1, colorMap_, mask, color);
    algorithm_dfs(row, col-1, colorMap_, mask, color);
}

void DrawingWidget::changeColor(QColor color) {
    this->currentColor_ = color;
}

void DrawingWidget::color_changed(QColor newColor) {
    this->currentColor_ = newColor;
}

void DrawingWidget::tool_changed() {
    QToolButton *clickedBtn = qobject_cast<QToolButton *>(sender());
    QString obj = clickedBtn->objectName();
    if(obj.compare("toolButton_pen") == 0){
        currentTool_ = PEN;
    }
    else if(obj.compare("toolButton_rubber") == 0){
        currentTool_ = RUBBER;
    }
    else if(obj.compare("toolButton_ink") == 0){
        currentTool_ = INK;
    }
    else if(obj.compare("toolButton_knife") == 0){
        currentTool_ = KNIFE;
    }
}
