//
// Created by Chris on 2023/2/2.
//

#include "HealerFriend.h"
#include "GameControl.h"

extern GameControl *game;

HealerFriend::HealerFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                           double atkInterval, const QString &appearFileName,
                           const QString &msFileName, QGraphicsRectItem *parent)
        : AbstractFriendObjects(blockNum, cost, healthLimit,
                                atk, def, atkInterval,
                                appearFileName, msFileName,
                                parent) {
    attackAreaAttr = new QVector<QPointF>;
    *attackAreaAttr << QPointF(0, -game->gridSizeY) << QPointF(game->gridSizeX * 4.0, 0)
                    << QPointF(game->gridSizeX * 4.0, game->gridSizeY * 2.0)
                    << QPointF(0, game->gridSizeY * 2.0);
}
