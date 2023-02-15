//
// Created by Chris on 2023/2/8.
//

#include "Cross.h"
#include "GameControl.h"
extern GameControl* game;

void Cross::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto friendObj = dynamic_cast<AbstractFriendObjects*>(parentObj);
    friendObj->getAttackArea()->setBrush(QBrush(Qt::NoBrush));
    game->gameStatus = GameControl::normal;
    delete friendObj->tick;
    friendObj->die(false);
    delete this;
}

Cross::Cross(QGraphicsItem * parentObj) :parentObj(parentObj) {
    setPixmap(QPixmap("://images/cross.png").scaled(80,80));
    setPos(parentObj->x()-110,parentObj->y()-110);
}
