//
// Created by Chris on 2023/1/15.
//

#ifndef TOWERDEF_GROUNDGRID_H
#define TOWERDEF_GROUNDGRID_H

#include "AbstractGrid.h"

class GroundGrid : public AbstractGrid {
private:
    bool deployability;

public:
    GroundGrid(int size, int sizeY, const QPointF &location, const QString &appr, bool _deployability,
               QGraphicsPixmapItem *parent = nullptr);

};


#endif //TOWERDEF_GROUNDGRID_H
