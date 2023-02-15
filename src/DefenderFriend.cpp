//
// Created by Chris on 2023/2/2.
//

#include "DefenderFriend.h"
#include "GameControl.h"

extern GameControl *game;

DefenderFriend::DefenderFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                               double atkInterval, const QString &appearanceFileName,
                               const QString &msFileName, QGraphicsRectItem *parent)
        : AbstractFriendObjects(blockNum, cost, healthLimit,
                                atk, def, atkInterval,
                                appearanceFileName, msFileName, parent) {
    attackAreaAttr = new QVector<QPointF>;
    *attackAreaAttr << QPointF(0, 0) << QPointF(game->gridSizeX, 0)
                    << QPointF(game->gridSizeX, game->gridSizeY)
                    << QPointF(0, game->gridSizeY);
}
