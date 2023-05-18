
#ifndef COLORGROUP_H
#define COLORGROUP_H

#include <QObject>
#include "colorwidget.h"

class ColorGroup : public QObject {
    Q_OBJECT
public:
    explicit ColorGroup(QObject *parent = nullptr);

    void addColorWidget(colorWidget *color);

private slots:
    void changeChecked();

private:
    QList<colorWidget *> colorWidgets;
    colorWidget* checkedColorWidget = nullptr;

};

#endif // COLORGROUP_H
