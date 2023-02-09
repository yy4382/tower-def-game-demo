//
// Created by Chris on 2023/2/7.
//

#include "Bullet.h"
#include "AbstractEnemy.h"
#include "AbstractFriendObjects.h"
#include "GameControl.h"
#include <QDebug>

extern GameControl *game;

Bullet::Bullet(QGraphicsItem *target, bulletEffect effect, QPointF initPos, double damageNum)
        : target(target), effect(effect), damageNum(damageNum),ifdestroy(false) {
    enemyTarget = dynamic_cast<AbstractEnemy *>(target);
    if (enemyTarget) {
        theTargetType = TargetEnemy;
        connect(enemyTarget, &AbstractEnemy::dieSignal, this, &Bullet::destroy,
                Qt::UniqueConnection);
    }
    friendTarget = dynamic_cast<AbstractFriendObjects *>(target);
    if (friendTarget) {
        theTargetType = TargetFriend;
        connect(friendTarget, &AbstractFriendObjects::dieSignal, this, &Bullet::destroy,
                Qt::UniqueConnection);
    }
    setPos(initPos);
    if(effect==damage) setPixmap(QPixmap("://images/bullet.png"));
    if(effect == heal) setPixmap(QPixmap("://images/bulletHeal.png"));
    speed = 25;
    moveTimer = new QTimer;
    moveTimer->start(20);
    connect(moveTimer, &QTimer::timeout, this, &Bullet::move);
}

void Bullet::move() {
//    QGraphicsItem* tempTarget = target;
    if(ifdestroy){
        delete this;
        return;
    }
    bool ifCollides;
    if (theTargetType == TargetEnemy) ifCollides = collidesWithItem(enemyTarget);
    else ifCollides = collidesWithItem(friendTarget);
    if (ifCollides) {
        this->disconnect();
        if (effect == damage) {
            auto *targetEnemy = dynamic_cast<AbstractEnemy *>(target);
            if (targetEnemy) targetEnemy->beAttacked(damageNum);
            else {
                auto *targetFriend = dynamic_cast<AbstractFriendObjects *>(target);
                if (targetFriend) targetFriend->beAttacked(damageNum);
            }
        } else if (effect == heal) {
            auto *targetEnemy = dynamic_cast<AbstractEnemy *>(target);
            if (targetEnemy) targetEnemy->beHealed(damageNum);
            else {
                auto *targetFriend = dynamic_cast<AbstractFriendObjects *>(target);
                if (targetFriend) targetFriend->beHealed(damageNum);
            }
        }
        delete this;
        return;
    }
    QLineF ln(this->pos(),
              QPointF(target->x() + game->gridSizeX / 2.0, target->y() + game->gridSizeY / 2.0));
    setRotation(-ln.angle());
    double theta = -1 * ln.angle();
    double dy = speed * qSin(qDegreesToRadians(theta));
    double dx = speed * qCos(qDegreesToRadians(theta));
    setPos(x() + dx, y() + dy);
}

Bullet::~Bullet() {
    delete moveTimer;
}

void Bullet::destroy() {
    ifdestroy = true;
}
