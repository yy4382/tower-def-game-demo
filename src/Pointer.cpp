//
// Created by Chris on 2023/2/4.
//
#include "Pointer.h"
#include "GameControl.h"
#include <QDebug>


extern GameControl *game;

Pointer::Pointer(QPointF pos,
                 double angle,
                 AbstractFriendObjects::directions dir,
                 AbstractFriendObjects *master)
        : dir(dir),
          master(master) {
    setPixmap(QPixmap("://images/pointer.png"));
    setRotation(angle);
    setPos(pos);
}

void Pointer::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    master->setDir(dir);
    delete[] game->pointers;
}
