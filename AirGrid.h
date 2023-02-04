//
// Created by Chris on 2023/1/16.
//

#ifndef TOWERDEF_AIRGRID_H
#define TOWERDEF_AIRGRID_H

#include "AbstractGrid.h"
class AirGrid :public AbstractGrid{
private:
    bool deployAbility;

public:
    AirGrid(int sizeX, int sizeY, const QPointF &location, const QString &appr,const QString &greenAppr, bool _deployability,
               QGraphicsPixmapItem *parent = nullptr);
    gridType getType() override { return airGrid; }
    bool getDeployAbility() override {return deployAbility;}
};


#endif //TOWERDEF_AIRGRID_H
