//
// Created by Chris on 2023/2/2.
//

#include "ShooterFriend.h"
#include "GameControl.h"
#include "Bullet.h"

extern GameControl *game;

ShooterFriend::ShooterFriend(int blockNum,
                             int cost,
                             double healthLimit,
                             double atk,
                             double def,
                             double atkInterval,
                             const QString &appearanceFileName,
                             const QString &HSFileName,
                             QGraphicsRectItem *parent)
        : AbstractFriendObjects(
        blockNum,
        cost,
        healthLimit,
        atk,
        def,
        atkInterval,
        appearanceFileName,
        HSFileName,
        parent) {
    attackAreaAttr = new QVector<QPointF>;
    *attackAreaAttr << QPointF(0, -game->gridSizeY)
                    << QPointF(game->gridSizeX * 4.0, -game->gridSizeY)
                    << QPointF(game->gridSizeX * 4.0, game->gridSizeY * 2.0)
                    << QPointF(0, game->gridSizeY * 2.0);
}

void ShooterFriend::attack(QGraphicsItem *target) {
    auto bullet = new Bullet(target, Bullet::damage, pos(), atk);
    scene()->addItem(bullet);
}
