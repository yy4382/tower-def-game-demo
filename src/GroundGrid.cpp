//
// Created by Chris on 2023/1/15.
//

#include <QCharRef>
#include "GroundGrid.h"

GroundGrid::GroundGrid(int sizeX,
                       int sizeY,
                       const QPointF &location,
                       const QString &appr,
                       const QString &greenAppr,
                       bool _deployability,
                       QGraphicsPixmapItem *parent)
        : AbstractGrid(location,
                       sizeX,
                       sizeY,
                       appr,
                       greenAppr,
                       parent),
          deployAbility(_deployability) {}
