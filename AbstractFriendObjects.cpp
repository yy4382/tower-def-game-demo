//
// Created by Chris on 2023/1/19.
//

#include "AbstractFriendObjects.h"
#include <utility>
#include "GameControl.h"
#include "Pointer.h"
#include <QDebug>
#include "AbstractEnemy.h"

extern GameControl *game;


AbstractFriendObjects::AbstractFriendObjects(int blockNum_, int cost_, double healthLimit_,
                                             double atk_, double def_,
                                             double atkInterval_, QString appearFileName,
                                             QString MSFileName,
                                             QGraphicsRectItem *parent)
        : QGraphicsRectItem(parent), healthLimit(healthLimit_), health(healthLimit_),
          atk(atk_), def(def_), atkInterval(atkInterval_/2.0), blockNum(blockNum_),
          cost(cost_), appearanceFileName(std::move(appearFileName)),
          MugShotFileName(std::move(MSFileName)),hpBar(nullptr),hpFullBar(nullptr),mode(MugShot) {
    //初始化图片
    appearance = new QGraphicsPixmapItem(this);
    setAppearance(MugShot);
    //设置碰撞箱大小
    setRect(0, 0, game->gridSizeX, game->gridSizeY);


    readyToAttack = true;

}

void AbstractFriendObjects::setAppearance(appearanceMode a) {
    if (a == MugShot) {
        QPixmap look = QPixmap{MugShotFileName}.scaledToHeight(game->gridSizeY);
        setPen(QPen(Qt::SolidLine));
        appearance->setPixmap(look);
        appearance->setOffset(0, 0);
        mode = MugShot;
    } else if (a == Fight) {
        QPixmap look = QPixmap{appearanceFileName}.scaledToHeight(game->gridSizeY * 1.2);
        appearance->setPixmap(look);
        appearance->setOffset((game->gridSizeX - look.width()) / 2.0,
                              -look.height() + game->gridSizeY * 0.8);
        setPen(QPen(Qt::NoPen));
        mode = Fight;
    } else if (a == FlippedFight) {
        QPixmap look =
                QPixmap{appearanceFileName}.scaledToHeight(game->gridSizeY * 1.2).transformed(
                        QTransform().scale(-1, 1));
        appearance->setPixmap(look);
        appearance->setOffset((game->gridSizeX - look.width()) / 2.0,
                              -look.height() + game->gridSizeY * 0.8);
        setPen(QPen(Qt::NoPen));
        mode = Fight;
    } else {
        //即为underCursor的情况
        QPixmap look = QPixmap{appearanceFileName}.scaledToHeight(game->gridSizeY * 1.2);
        appearance->setPixmap(look);
        appearance->setOffset((double) -game->gridSizeX / 2, (double) -game->gridSizeY / 2);
        mode = underCursor;
        setPen(QPen(Qt::NoPen));
    }
}

void AbstractFriendObjects::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (game->gameStatus == GameControl::normal && mode == MugShot) {
        game->cursor = this;
        setAppearance(underCursor);
        game->gameStatus = GameControl::build;
        if (getType() == Defender || getType() == Vanguard) game->setGroundGridGreen(true);
        else game->setAirGridGreen(true);
    } else if (game->gameStatus == GameControl::normal && mode == Fight) {
        game->gameStatus = GameControl::showDetail;
        attackArea->setBrush(QBrush(QColor(Qt::blue), Qt::CrossPattern));
        attackArea->setOpacity(20);
        tick = new Tick(this);
        cross = new Cross(this);
        scene()->addItem(tick);
        scene()->addItem(cross);
    }
}

void AbstractFriendObjects::intoFight(QPointF position) {
    setPos(position);
    setAppearance(Fight);
    showPointer();
}

void AbstractFriendObjects::showPointer() {
    game->pointers = new Pointer[4]{{mapToScene(game->gridSizeX + 30,
                                                (double) (game->gridSizeY - 100) /
                                                2),      0,   right, this},
                                    {mapToScene(-30, (double) (game->gridSizeY + 100) /
                                                     2), 180, left,  this},
                                    {mapToScene((double) (game->gridSizeX - 100) / 2,
                                                -30),    270, up,    this},
                                    {mapToScene((double) (game->gridSizeX + 100) / 2,
                                                game->gridSizeY + 30),
                                                         90,  down,  this}};
    for (int i = 0; i < 4; i++) {
        scene()->addItem(game->pointers + i);
    }
}

void AbstractFriendObjects::setDir(directions dir) {
    //appearance
    if (dir == right) {
        attackArea = new QGraphicsPolygonItem(QPolygonF(*attackAreaAttr), this);
        attackArea->setPen(QPen(Qt::NoPen));
    } else if (dir == left) {
        setAppearance(FlippedFight);
        attackArea = new QGraphicsPolygonItem(QPolygonF(*attackAreaAttr), this);
        attackArea->setRotation(180);
        attackArea->setPos(game->gridSizeX, game->gridSizeY);
        attackArea->setPen(QPen(Qt::NoPen));
//        attackArea->setBrush(QBrush(QColor(Qt::blue)));
    } else if (dir == up) {
        attackArea = new QGraphicsPolygonItem(QPolygonF(*attackAreaAttr), this);
        attackArea->setRotation(270);
        attackArea->setPos(0, game->gridSizeY);
        attackArea->setPen(QPen(Qt::NoPen));
    } else if (dir == down) {
        attackArea = new QGraphicsPolygonItem(QPolygonF(*attackAreaAttr), this);
        attackArea->setRotation(90);
        attackArea->setPos(game->gridSizeX, 0);
        attackArea->setPen(QPen(Qt::NoPen));
    }

    initHpBar();
    game->gameStatus = GameControl::normal;
    connect(&friendTimer, SIGNAL(timeout()), this, SLOT(acquireTarget()));
    friendTimer.start(20);
}

AbstractFriendObjects::friendType AbstractFriendObjects::intToFriendType(int type) {
    switch (type) {
        case 1:
            return AbstractFriendObjects::Shooter;
        case 2:
            return AbstractFriendObjects::Defender;
        case 3:
            return AbstractFriendObjects::Healer;
        case 4:
            return AbstractFriendObjects::Vanguard;
        case 5:
            return AbstractFriendObjects::SplashCaster;
        default:
            return null;
    }
}

void AbstractFriendObjects::acquireTarget() {
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
    double closestDist = 10000;
    AbstractEnemy *target;
    for (auto i: collidingItemList) {
        auto *enemy = dynamic_cast<AbstractEnemy *>(i);
        if (enemy && enemy->distanceLeft() <= closestDist) {
            target = enemy;
            closestDist = enemy->distanceLeft();
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

void AbstractFriendObjects::attack(QGraphicsItem *target0) {
    auto *target = dynamic_cast<AbstractEnemy *>(target0);
    target->beAttacked(atk);
}

void AbstractFriendObjects::setHpBar() {
    hpBar->setRect(10, game->gridSizeY - 20, health / healthLimit * (game->gridSizeX - 20), 10);
}

void AbstractFriendObjects::initHpBar() {
    hpFullBar = new QGraphicsRectItem(this);
    hpFullBar->setZValue(10);
    hpFullBar->setRect(10, game->gridSizeY - 20, health / healthLimit * (game->gridSizeX - 20), 10);
    hpBar = new QGraphicsRectItem(this);
    hpBar->setZValue(10);
    setHpBar();
    QBrush hpBrush(Qt::red, Qt::SolidPattern);
    hpBar->setBrush(hpBrush);
}

void AbstractFriendObjects::beAttacked(double damage) {
    double realDamage = damage - def;
    if (realDamage < damage * 0.05) realDamage = damage * 0.05;
    health = health - realDamage;
    setHpBar();
    if (health <= 0) die();
}

void AbstractFriendObjects::die() {
//    emit dieSignal();
    for (auto i: blockList) i->free();
    setAppearance(MugShot);
    setPos(location);
    health = healthLimit;
    delete hpBar;
    delete hpFullBar;
    friendTimer.stop();
    attackTimer.stop();
}

void AbstractFriendObjects::beHealed(double damage) {
    health = health+damage;
    if(health>healthLimit) health = healthLimit;
    setHpBar();
}

