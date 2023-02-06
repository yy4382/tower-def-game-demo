//
// Created by Chris on 2023/2/4.
//

#ifndef TOWERDEF_POINTER_H
#define TOWERDEF_POINTER_H

#include "AbstractFriendObjects.h"
#include <QGraphicsPixmapItem>
#include <QDebug>

class Pointer : public QObject, public QGraphicsPixmapItem{
public:
    Pointer(QPointF pos, double angle, AbstractFriendObjects::directions dir, AbstractFriendObjects* master);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    AbstractFriendObjects::directions dir;
    AbstractFriendObjects* master;
};


#endif //TOWERDEF_POINTER_H
