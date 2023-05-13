#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

class DrawingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingWidget(QWidget* parent = nullptr);

    void exportDrawingBoard();

    QList<QColor> importDrawingBoard();

    void resetDrawingBoard();

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent*) override;

private:
    void fillGrid();

    void drawPixel(const QPoint& pos);

    void drawPixel(int row, int col, QColor color);

private slots:
    void color_changed(QColor newColor);

private:
    QImage image_{529, 529, QImage::Format_RGB32};
    QPoint lastPos_;
    QList<QColor> colorMap_ = QList<QColor>();
    QColor currentColor_ = QColor(0, 0, 0);

};

#endif // DRAWINGWIDGET_H
