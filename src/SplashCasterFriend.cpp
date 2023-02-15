//
// Created by Chris on 2023/2/2.
//

#include "SplashCasterFriend.h"
#include "GameControl.h"

extern GameControl *game;

SplashCasterFriend::SplashCasterFriend(int blockNum, int cost, double healthLimit, double atk,
                                       double def, double atkInterval,
                                       const QString &appearFileName, const QString &msFileName,
                                       QGraphicsRectItem *parent)
        : AbstractFriendObjects(
        blockNum, cost, healthLimit, atk, def,
        atkInterval, appearFileName, msFileName, parent) {
    attackAreaAttr = new QVector<QPointF>;
    *attackAreaAttr << QPointF(0, -game->gridSizeY) << QPointF(game->gridSizeX * 3.0, -game->gridSizeY)
                    << QPointF(game->gridSizeX * 3.0, game->gridSizeY * 2.0)
                    << QPointF(0, game->gridSizeY * 2.0);
}

void SplashCasterFriend::attack(QGraphicsItem *target) {
    auto area = new QGraphicsEllipseItem;
    scene()->addItem(area);
    area->setPen(QPen(Qt::NoPen));
    area->setPos(target->pos());
    area->setRect(0,0,game->gridSizeX*1.1,game->gridSizeY*1.1);
    image = new QGraphicsPixmapItem(QPixmap("://images/explosion.png").scaled(game->gridSizeX*1.1,game->gridSizeY*1.1));
    image->setPos(target->pos());
    scene()->addItem(image);
    imageTimer = new QTimer;
    imageTimer->setSingleShot(true);
    imageTimer->start(100);
    connect(imageTimer, &QTimer::timeout, [&](){
        delete image;
        delete imageTimer;
    });
    QList<QGraphicsItem *> items = area->collidingItems();
    delete area;
    for(auto i:items){
        auto* enemy = dynamic_cast<AbstractEnemy*>(i);
        if(enemy) enemy->beAttacked(atk);
    }

}
