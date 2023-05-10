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

    QList<QColor> print();
    QList<QColor> import();

    QPixmap getPixmap() const;

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent*) override;

    void enterEvent(QEvent*) override;

    void leaveEvent(QEvent*) override;


private:
    void fillGrid();

    void highlightCell(const QPoint& pos);

    void drawPixel(const QPoint& pos);

    void drawPixel(int row, int col, QColor color);

    void drawLine(const QPoint& from, const QPoint& to);

private slots:
    void color_changed(QColor newColor);

private:
    QImage image_{513, 513, QImage::Format_RGB32};
    QPoint lastPos_;
    QList<QColor> colorMap_;
    QColor currentColor_;

};

#endif // DRAWINGWIDGET_H
