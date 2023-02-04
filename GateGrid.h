//
// Created by Chris on 2023/1/16.
//

#ifndef TOWERDEF_GATEGRID_H
#define TOWERDEF_GATEGRID_H

#include "AbstractGrid.h"

class GateGrid : public AbstractGrid {
public:
    GateGrid(int sizeX, int sizeY, const QPointF &location, const QString &appr,
             const QString &greenAppr, const QString &type, QGraphicsPixmapItem *parent = nullptr);

    gridType getType() override { if (gateType)return blueGateGrid; else return redGateGrid; }

private:
    int gateType; // 0->redGate, 1->blueGate
};


#endif //TOWERDEF_GATEGRID_H
