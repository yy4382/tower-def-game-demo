//
// Created by Chris on 2023/1/16.
//

#include "GateGrid.h"


GateGrid::GateGrid(int sizeX, int sizeY, const QPointF &location, const QString &appr,
                   const QString &type,
                   QGraphicsPixmapItem *parent) : AbstractGrid(location, sizeX, sizeY, appr) {
    if (type == "blueGate") gateType = 1;
    if (type == "redGate") gateType = 2;
}
