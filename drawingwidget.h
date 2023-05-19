#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

enum e_tools {
    PEN = 0,
    RUBBER,
    INK,
    KNIFE
};

class DrawingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawingWidget(QWidget* parent = nullptr, QColor color = QColor(0,0,0));

    void exportDrawingBoard();

    QList<QColor> importDrawingBoard();

    void resetDrawingBoard();

    void generateCode();

    void sendDrawingBoard();

    void changeColor(QColor color);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent*) override;

private:
    void fillGrid();

    void drawPixel(const QPoint& pos);

    void drawPixel(int row, int col, QColor color);

    void erasePixel(const QPoint& pos);

    void slakePixel(const QPoint& pos);

    void dyePixel(const QPoint& pos);

    void algorithm_dfs(int row, int col, QList<QColor> &map, QList<int> &mask, QColor color);

private slots:
    void color_changed(QColor newColor);

    void tool_changed();

private:
    QImage image_{529, 529, QImage::Format_RGB32};
    QList<QColor> colorMap_ = QList<QColor>();
    QColor currentColor_ = QColor(0, 0, 0);
    int currentTool_ = PEN;

signals:
    void dataReady(const QByteArray* data);

};

#endif // DRAWINGWIDGET_H
