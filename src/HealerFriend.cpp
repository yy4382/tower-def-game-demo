//
// Created by Chris on 2023/2/2.
//

#include "HealerFriend.h"
#include "GameControl.h"
#include "Bullet.h"

extern GameControl *game;

HealerFriend::HealerFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                           double atkInterval, const QString &appearFileName,
                           const QString &msFileName, QGraphicsRectItem *parent)
        : AbstractFriendObjects(blockNum, cost, healthLimit,
                                atk, def, atkInterval,
                                appearFileName, msFileName,
                                parent) {
    attackAreaAttr = new QVector<QPointF>;
    *attackAreaAttr << QPointF(0, -game->gridSizeY) << QPointF(game->gridSizeX * 4.0, -game->gridSizeY)
                    << QPointF(game->gridSizeX * 4.0, game->gridSizeY * 2.0)
                    << QPointF(0, game->gridSizeY * 2.0);
}

void HealerFriend::attack(QGraphicsItem *target) {
    auto bullet = new Bullet(target, Bullet::heal, pos(), atk);
    scene()->addItem(bullet);
}

void HealerFriend::acquireTarget() {
    bool hasTarget = false;
    if (blockList.size() < blockNum) {
        QList<QGraphicsItem *> collidingItemList = this->collidingItems();
        for (auto i: collidingItemList) {
            auto *enemy = dynamic_cast<AbstractEnemy *>(i);
            if (enemy && !enemy->getBlocked()) {
                blockList.append(enemy);
                enemy->blocked(this);
                connect(enemy, &AbstractEnemy::dieSignal, [&, enemy]() {
                    for (int i = 0; i < blockList.size(); i++)
                        if (blockList.at(i) == enemy)blockList.removeAt(i);
                });
            }
        }
    }
    QList<QGraphicsItem *> collidingItemList = attackArea->collidingItems();
    double healthPercent = 1;
    AbstractFriendObjects *target;
    for (auto i: collidingItemList) {
        auto *fTarget = dynamic_cast<AbstractFriendObjects *>(i);
        if (fTarget && fTarget->getMode() ==AbstractFriendObjects::Fight &&
            fTarget->getHealthPercent() < healthPercent) {
            target = fTarget;
            healthPercent = fTarget->getHealthPercent();
            hasTarget = true;
        }
    }
    if (hasTarget && readyToAttack) {
        attackTimer.stop();
        attack(target);
        attackTimer.start((int) atkInterval);
        readyToAttack = false;
        connect(&attackTimer, &QTimer::timeout, [this]() { readyToAttack = true; });
    }
}
