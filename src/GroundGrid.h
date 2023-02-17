//
// Created by Chris on 2023/1/15.
//

#ifndef TOWERDEF_GROUNDGRID_H
#define TOWERDEF_GROUNDGRID_H

#include "AbstractGrid.h"

class GroundGrid : public AbstractGrid {
private:
    bool deployAbility;

public:
    GroundGrid(int size,
               int sizeY,
               const QPointF &location,
               const QString &appr,
               const QString &greenAppr,
               bool _deployability,
               QGraphicsPixmapItem *parent = nullptr);
    gridType getType() override { return groundGrid; }
    bool getDeployAbility() override {return deployAbility;}
};


#endif //TOWERDEF_GROUNDGRID_H
