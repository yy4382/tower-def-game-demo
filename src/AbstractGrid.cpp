//
// Created by Chris on 2023/1/15.
//

#include "AbstractGrid.h"
#include <QDebug>

AbstractGrid::AbstractGrid(const QPointF &_location, int _sizeX, int _sizeY, const QString &appr,
                           const QString &greenAppr, QGraphicsPixmapItem *parent)
        : QGraphicsPixmapItem(parent),
          sizeX(_sizeX),sizeY(_sizeY), location(_location), appearance(QPixmap(appr)),greenAppearance(QPixmap(greenAppr)) {
    setPos(location);
    setPixmap(appearance.scaled(sizeX, sizeY, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

void AbstractGrid::setGreen() {
    if (getDeployAbility())
        setPixmap(greenAppearance.scaled(sizeX, sizeY, Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation));

}

void AbstractGrid::setDefault() {
    if (getDeployAbility())
        setPixmap(appearance.scaled(sizeX, sizeY, Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation));
}
