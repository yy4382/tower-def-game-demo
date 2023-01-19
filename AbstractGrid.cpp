//
// Created by Chris on 2023/1/15.
//

#include "AbstractGrid.h"
#include <QDebug>

AbstractGrid::AbstractGrid(const QPointF &_location, int _sizeX, int _sizeY, const QString &appr,
                           QGraphicsPixmapItem *parent)
        : QGraphicsPixmapItem(parent),
          sizeX(_sizeX),sizeY(_sizeY), location(_location), appearance(QPixmap(appr)) {
    setPos(location);
    setPixmap(appearance.scaled(sizeX, sizeY, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
