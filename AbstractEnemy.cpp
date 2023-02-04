//
// Created by Chris on 2023/1/17.
//

#include "AbstractEnemy.h"
#include "GameControl.h"
#include <QDebug>

extern GameControl *game;

AbstractEnemy::AbstractEnemy(const enemyBasicAttr &a) : appearanceFileName(a.appearance),
                                                        path(a.path), pathIndex(0),
                                                        healthLimit(a.healthLimit),
                                                        health(healthLimit),
                                                        atk(a.atk), def(a.def), speed(a.speed),
                                                        isFlyingObject(a.isFlyingObject) {
    //设置位置和大小
    setPos(path[pathIndex].x(), path[pathIndex].y());
    setRect(game->gridSizeX / 2 - 2.5, game->gridSizeY / 2 - 2.5, 5, 5);
    bool ifFlipped = false;
    QLineF ln(pos(),path[pathIndex+1]);
    if (ln.angle() > 90 && ln.angle() < 270) ifFlipped = true;
    appearance = new QGraphicsPixmapItem(this);
    setAppearance(ifFlipped);

    //敌人活动的计时器
    enemyMoveTimer = new QTimer();
    connect(enemyMoveTimer, SIGNAL(timeout()), this, SLOT(move()));
    enemyMoveTimer->start(20);
    pathIndex++;
}

void AbstractEnemy::move() {
    QLineF ln(pos(), path[pathIndex]);
    if (ln.length() <= 5) {
        pathIndex++;
        if (pathIndex >= path.size()) {
            die(0);
            return;
        }
        ln = QLineF(pos(), path[pathIndex]);
        if (ln.angle() > 90 && ln.angle() < 270) {
            setAppearance(true);
        }else {
            setAppearance(false);
        }
    }
    double theta = -1 * ln.angle();
    double dy = speed * qSin(qDegreesToRadians(theta));
    double dx = speed * qCos(qDegreesToRadians(theta));
    setPos(x() + dx, y() + dy);
}

void AbstractEnemy::die(int type) {
    delete enemyMoveTimer;
    delete this;
}

void AbstractEnemy::setAppearance(bool ifFlipped) {
    QPixmap look {appearanceFileName};
    if(ifFlipped) look = look.transformed(QTransform().scale(-1,1));
    look = look.scaledToWidth(game->gridSizeX);
    appearance->setPixmap(look);
    appearance->setOffset(0,-look.height()+game->gridSizeY*0.8);
}
