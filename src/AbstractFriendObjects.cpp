//
// Created by Chris on 2023/1/19.
//

#include "AbstractFriendObjects.h"
#include <utility>
#include "GameControl.h"
#include "Pointer.h"
#include <QDebug>
//#include <dbghelp.h>
#include "AbstractEnemy.h"

extern GameControl *game;


AbstractFriendObjects::AbstractFriendObjects(int blockNum_,
                                             int cost_,
                                             double healthLimit_,
                                             double atk_,
                                             double def_,
                                             double atkInterval_,
                                             QString appearFileName,
                                             QString MSFileName,
                                             QGraphicsRectItem *parent)
        : QGraphicsRectItem(parent),
          healthLimit(healthLimit_),
          health(healthLimit_),
          atk(atk_),
          def(def_),
          atkInterval(atkInterval_ / 2.0),
          blockNum(blockNum_),
          cost(cost_),
          appearanceFileName(std::move(appearFileName)),
          headShotFileName(std::move(MSFileName)),
          hpBar(nullptr),
          hpFullBar(nullptr),
          mode(HeadShot),
          readyToAttack(true) {
    //初始化图片
    appearance = new QGraphicsPixmapItem(this);
    setAppearance(HeadShot);
    //设置碰撞箱大小
    setRect(0, 0, game->gridSizeX, game->gridSizeY);
}

void AbstractFriendObjects::setAppearance(appearanceMode a) {
    if (a == HeadShot) {
        QPixmap look = QPixmap{headShotFileName}
                .scaledToHeight(game->gridSizeY);
        setPen(QPen(Qt::SolidLine));
        appearance->setPixmap(look);
        appearance->setOffset(0, 0);
        showCost();
        mode = HeadShot;
    } else if (a == Fight) {
        QPixmap look = QPixmap{appearanceFileName}
                .scaledToHeight(game->gridSizeY * 1.2);
        appearance->setPixmap(look);
        appearance->setOffset((game->gridSizeX - look.width()) / 2.0,
                              -look.height() + game->gridSizeY * 0.8);
        setPen(QPen(Qt::NoPen));
        mode = Fight;
    } else if (a == FlippedFight) {
        QPixmap look =
                QPixmap{appearanceFileName}
                        .scaledToHeight(game->gridSizeY * 1.2)
                        .transformed(QTransform().scale(-1, 1));
        appearance->setPixmap(look);
        appearance->setOffset((game->gridSizeX - look.width()) / 2.0,
                              -look.height() + game->gridSizeY * 0.8);
        setPen(QPen(Qt::NoPen));
        mode = Fight;
    } else {
        //即为underCursor的情况
        QPixmap look = QPixmap{appearanceFileName}
                .scaledToHeight(game->gridSizeY * 1.2);
        appearance->setPixmap(look);
        appearance->setOffset((double) -game->gridSizeX / 2, (double) -game->gridSizeY / 2);
        hideCost();
        mode = underCursor;
        setPen(QPen(Qt::NoPen));
    }
}

void AbstractFriendObjects::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (game->gameStatus == GameControl::normal &&
        mode == HeadShot &&
        game->costBoard->minusCost(cost)) {
        //点击待部署栏，开始部署
        game->cursor = this;
        setAppearance(underCursor);
        game->gameStatus = GameControl::build;
        if (getType() == Defender || getType() == Vanguard) game->setGroundGridGreen(true);
        else game->setAirGridGreen(true);
    } else if (game->gameStatus == GameControl::normal && mode == Fight) {
        //点击场地中的队员，显示信息
        game->gameStatus = GameControl::showDetail;
        //显示攻击范围
        attackArea->setBrush(QBrush(QColor(Qt::blue), Qt::FDiagPattern));
        attackArea->setOpacity(0.8);
        //显示勾和叉
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
                                                (game->gridSizeY - 100) / 2),
                                            0,
                                            right,
                                            this},
                                    {mapToScene(-30, (game->gridSizeY + 100) / 2),
                                            180,
                                            left,
                                            this},
                                    {mapToScene((game->gridSizeX - 100) / 2, -30),
                                            270,
                                            up,
                                            this},
                                    {mapToScene((game->gridSizeX + 100) / 2,
                                                game->gridSizeY + 30),
                                            90,
                                            down,
                                            this}};
    for (int i = 0; i < 4; i++)
        scene()->addItem(game->pointers + i);
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
    connect(&friendTimer, SIGNAL(timeout()), this, SLOT(friendAction()), Qt::UniqueConnection);
    friendTimer.start(20);
    readyToAttack = true;
    emit fightStart();
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
        connect(&attackTimer, &QTimer::timeout, [this]() {
            readyToAttack = true;
        });
    }
}

void AbstractFriendObjects::attack(QGraphicsItem *target0) {
    auto *target = dynamic_cast<AbstractEnemy *>(target0);
    target->beAttacked(atk);
}

void AbstractFriendObjects::setHpBar() {
    hpBar->setRect(10,
                   game->gridSizeY - 20,
                   health / healthLimit * (game->gridSizeX - 20),
                   10);
}

void AbstractFriendObjects::initHpBar() {
    hpFullBar = new QGraphicsRectItem(this);
    hpFullBar->setZValue(10);
    hpFullBar->setRect(10,
                       game->gridSizeY - 20,
                       health / healthLimit * (game->gridSizeX - 20),
                       10);
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
    if (health <= 0) die(true);
}

void AbstractFriendObjects::die(bool beKilled) {
    if (!beKilled) game->costBoard->addCost(cost / 2);
    if (cost * 1.5 <= 99) cost *= 1.5;
    for (auto i: blockList) i->free();
    setAppearance(HeadShot);
    setPos(location);
    health = healthLimit;
    delete hpBar;
    delete hpFullBar;
    friendTimer.stop();
    attackTimer.stop();
    emit fightStop();
    emit dieSignal();
}

void AbstractFriendObjects::beHealed(double damage) {
    health = health + damage;
    if (health > healthLimit) health = healthLimit;
    setHpBar();
}

void AbstractFriendObjects::hideCost() {
    delete costBg;
    delete costDisplay;
}

void AbstractFriendObjects::showCost() {
    costBg = new QGraphicsRectItem(0, 0, 40, 40, this);
    costBg->setPen(QPen(Qt::NoPen));
    costBg->setBrush(QBrush(Qt::gray));
    costBg->setOpacity(0.8);
    costDisplay = new QGraphicsTextItem(this);
    costDisplay->setPlainText(QString::number(cost));
    costDisplay->setFont(QFont("Consolas", 18));
}

void AbstractFriendObjects::renewBlockList() {
    if (blockList.size() < blockNum) {
        QList<QGraphicsItem *> collidingItemList = this->collidingItems();
        for (auto i: collidingItemList) {
            auto *enemy = dynamic_cast<AbstractEnemy *>(i);
            if (enemy && !enemy->getBlocked()) {
                blockList.append(enemy);
                enemy->blocked(this);
                connect(enemy, &AbstractEnemy::dieSignal,
                        [&, enemy]() {
                            for (int i = 0; i < blockList.size(); i++)
                                if (blockList.at(i) == enemy)blockList.removeAt(i);
                        });
            }
        }
    }
}

void AbstractFriendObjects::friendAction() {
    renewBlockList();
    acquireTarget();
}
