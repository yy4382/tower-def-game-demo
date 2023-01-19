//
// Created by Chris on 2023/1/16.
//

#ifndef TOWERDEF_AIRGRID_H
#define TOWERDEF_AIRGRID_H

#include "AbstractGrid.h"
class AirGrid :public AbstractGrid{
private:
    bool deployability;

public:
    AirGrid(int sizeX, int sizeY, const QPointF &location, const QString &appr, bool _deployability,
               QGraphicsPixmapItem *parent = nullptr);

};


#endif //TOWERDEF_AIRGRID_H
