
#include "colorgroup.h"
#include <QDebug>

ColorGroup::ColorGroup(QObject *parent) : QObject{parent} {

}

void ColorGroup::addColorWidget(colorWidget *color) {
    colorWidgets.append(color);
    connect(color, SIGNAL(change()), this, SLOT(changeChecked()));
}

void ColorGroup::changeChecked() {
//    qDebug() << "ColorGroup::changeChecked";
    if(qobject_cast<colorWidget *>(sender()) != checkedColorWidget){
        checkedColorWidget = qobject_cast<colorWidget *>(sender());
        for(auto color : qAsConst(colorWidgets)){
            if(color == checkedColorWidget){
                color->setChecked(true);
            }
            else{
                color->setChecked(false);
            }
        }
    }

}

