//
// Created by Chris on 2023/2/2.
//

#include "VanguardFriend.h"
#include "GameControl.h"

extern GameControl *game;
VanguardFriend::VanguardFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                               double atkInterval, const QString &appearFileName,
                               const QString &msFileName, QGraphicsRectItem *parent)
        : AbstractFriendObjects(blockNum, cost, healthLimit,
                                atk, def, atkInterval,
                                appearFileName, msFileName,
                                parent) {
    attackAreaAttr = new QVector<QPointF>;
    *attackAreaAttr << QPointF(0, 0) << QPointF(game->gridSizeX*2.0, 0)
                    << QPointF(game->gridSizeX*2.0, game->gridSizeY)
                    << QPointF(0, game->gridSizeY);
}
