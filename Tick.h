//
// Created by Chris on 2023/2/8.
//

#ifndef TOWERDEF_TICK_H
#define TOWERDEF_TICK_H
#include "QGraphicsPixmapItem"

class Tick : public QGraphicsPixmapItem{
public:
    Tick(QGraphicsItem*);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QGraphicsItem* parentObj;
};


#endif //TOWERDEF_TICK_H
