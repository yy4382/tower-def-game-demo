//
// Created by Chris on 2023/2/8.
//

#include "Tick.h"
#include "GameControl.h"
extern GameControl* game;

Tick::Tick(QGraphicsItem* parentObj): parentObj(parentObj) {
    setPixmap(QPixmap("://images/tick.png").scaled(80,80));
    setPos(parentObj->x()+game->gridSizeX+30,parentObj->y()+game->gridSizeY+30);
}
void Tick::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto friendObj = dynamic_cast<AbstractFriendObjects*>(parentObj);
    friendObj->getAttackArea()->setBrush(QBrush(Qt::NoBrush));
    game->gameStatus = GameControl::normal;
    delete friendObj->cross;
    delete this;
}