//
// Created by Chris on 2023/2/7.
//

#ifndef TOWERDEF_BULLET_H
#define TOWERDEF_BULLET_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "AbstractFriendObjects.h"
#include "AbstractEnemy.h"

class Bullet : public QObject, public QGraphicsPixmapItem {
Q_OBJECT
public:
    enum bulletEffect {
        damage, heal
    };
    enum targetType {
        TargetFriend, TargetEnemy
    };

    Bullet(QGraphicsItem *target, bulletEffect effect, QPointF initPos, double damageNum);

    ~Bullet() override;

private:
    double speed, damageNum;
    QGraphicsItem *target;
    AbstractFriendObjects *friendTarget;
    AbstractEnemy *enemyTarget;
    QTimer *moveTimer;
    bulletEffect effect;
    targetType theTargetType;
    bool ifdestroy;
public slots:

    void move();

    void destroy();
};


#endif //TOWERDEF_BULLET_H
