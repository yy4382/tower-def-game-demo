//
// Created by Chris on 2023/1/15.
//

#include "GroundGrid.h"

GroundGrid::GroundGrid(int sizeX, int sizeY, const QPointF &location, const QString &appr,
                       bool _deployability,
                       QGraphicsPixmapItem *parent)
        : AbstractGrid(location, sizeX, sizeY, appr, parent), deployability(_deployability) {}
