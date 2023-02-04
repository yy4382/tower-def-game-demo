//
// Created by Chris on 2023/1/15.
//

#include "GameControl.h"
#include <QDebug>

GameControl::GameControl() : curBirthEnemy(0) {
    readMapInfo("testMap");
    readEnemyInfo("testMap");
    readFriendInfo("testMap");

    //set view rect
    setFixedSize(viewWidth, viewHeight);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create and set a scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, viewWidth, viewHeight);
    setScene(scene);

    //mouse event
    setMouseTracking(true);
    cursor = nullptr;
    gameStatus = normal;

    initBackground();
    initEnemy();
    auto friendInit = new QTimer();
    friendInit->setSingleShot(true);
    connect(friendInit, SIGNAL(timeout()), this, SLOT(initFriend()));
    friendInit->start(10);
}

void GameControl::initBackground() {
    //init grids
    double x = marginLeftAndRight, y = marginUp;
    int index = 0;
    for (int i = 0; i < rowY; i++) {
        for (int j = 0; j < rowX; j++) {
            if ((gridAttr + index)->type != "null") {
                //Gate
                auto *grid = new GateGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                          (gridAttr + index)->appearance,
                                          (gridAttr + index)->greenAppearance,
                                          (gridAttr + index)->type);
                scene->addItem(grid);
                gridList << grid;
            } else if ((gridAttr + index)->ground == 1) {
                //ground grid
                auto *grid = new GroundGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                            (gridAttr + index)->appearance,
                                            (gridAttr + index)->greenAppearance,
                                            (gridAttr + index)->deployability);
                scene->addItem(grid);
                gridList << grid;
            } else if ((gridAttr + index)->ground == 0) {
                // air grid
                auto *grid = new AirGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                         (gridAttr + index)->appearance,
                                         (gridAttr + index)->greenAppearance,
                                         (gridAttr + index)->deployability);
                scene->addItem(grid);
                gridList << grid;
            }
            x += gridSizeX;
            index++;
        }
        y += gridSizeY;
        x = 0;
    }

    //init pause btn
    auto *pauseBtn = new QGraphicsPixmapItem(
            QPixmap("://images/pauseBtn.png").scaled(marginUp - 10, marginUp - 10));
    scene->addItem(pauseBtn);
    pauseBtn->setPos(QPointF(viewWidth - marginUp - 15, 5));


}

void GameControl::readMapInfo(const QString &mapName) {
    //过渡代码  MapInfo从文件读入
    viewWidth = 1600;
    viewHeight = 1100;
    gridSizeX = 160;
    gridSizeY = 160;
    marginLeftAndRight = 0;
    marginUp = 70;
    QString gd(":/images/DeployableGroundGrid1.png"),
            gu(":/images/UndeployableGroundGrid1.png"),
            rg(":/images/DeployableRedGateGrid1.png"),
            bg(":/images/DeployableBlueGateGrid1.png"),
            ad(":/images/DeployableAirGrid1.png"),
            au(":/images/UndeployableAirGrid1.png");
    GridAttr gda(gd, "://images/DeployableGroundGridGreen.png", true, true),
            gua(gu, gu, false, true),
            ada(ad, "://images/DeployableAirGridGreen.png", true, false),
            aua(au, au, false, false),
            rga(rg, rg, false, false, "redGate"),
            bga(bg, bg, false, false, "blueGate");
    rowX = 10;
    rowY = 5;
    gridAttr = new GridAttr[50]
            {
                    aua, ada, ada, ada, gda, gda, gda, gda, gua, rga,
                    aua, ada, gda, gda, gda, ada, ada, ada, aua, aua,
                    bga, gda, gda, ada, gda, gda, gda, gda, gua, rga,
                    aua, ada, gda, gda, gda, ada, ada, ada, aua, aua,
                    aua, ada, ada, ada, gda, gda, gda, gda, gua, rga
            };
}

QPointF GameControl::indexToCoordinate(int x, int y) const {
    double xx = marginLeftAndRight, yy = marginUp;
    for (int i = 0; i < x; i++) xx += gridSizeX;
    for (int i = 0; i < y; i++) yy += gridSizeY;
    return {xx, yy};
}

void GameControl::readEnemyInfo(const QString &mapName) {
    enemyTotalNum = 10;
    enemyAttrList = new QList<enemyBasicAttr *>();

    // 过渡代码  readEnemyInfo 从文件读入
    QList<QPointF> typeUp, typeMiddleUp, typeMiddleDown, typeDown;
    typeUp << indexToCoordinate(9, 0) << indexToCoordinate(4, 0) << indexToCoordinate(4, 1)
           << indexToCoordinate(2, 1) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    typeMiddleUp << indexToCoordinate(9, 2) << indexToCoordinate(4, 2) << indexToCoordinate(4, 1)
                 << indexToCoordinate(2, 1) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    typeMiddleDown << indexToCoordinate(9, 2) << indexToCoordinate(4, 2) << indexToCoordinate(4, 3)
                   << indexToCoordinate(2, 3) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    typeDown << indexToCoordinate(9, 4) << indexToCoordinate(4, 4) << indexToCoordinate(4, 3)
             << indexToCoordinate(2, 3) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    double speed1 = 3;
    auto *a1 = new enemyBasicAttr{3000, typeUp, 1000, 185, 0,
                                  speed1, false, "://images/enemyDemo1.png"};
    auto *a2 = new enemyBasicAttr{6000, typeDown, 1000, 100, 0,
                                  speed1, false, "://images/enemyDemo1.png"};
    auto *a3 = new enemyBasicAttr{6500, typeDown, 1000, 100, 0,
                                  speed1, false, "://images/enemyDemo1.png"};
    auto *a4 = new enemyBasicAttr{9000, typeMiddleUp, 1000, 100,
                                  0, speed1, false, "://images/enemyDemo1.png"};
    auto *a5 = new enemyBasicAttr{11500, typeMiddleUp, 1000, 100,
                                  0, speed1, false, "://images/enemyDemo1.png"};
    auto *a6 = new enemyBasicAttr{9500, typeMiddleDown, 1000, 100,
                                  0, speed1, false, "://images/enemyDemo1.png"};
    auto *a7 = new enemyBasicAttr{12000, typeMiddleDown, 1000, 100,
                                  0, speed1, false, "://images/enemyDemo1.png"};
    auto *b8 = new enemyBasicAttr{16000, typeUp, 1700, 250, 50,
                                  speed1 * 1.1, false, "://images/enemyDemo2.png"};
    auto *b9 = new enemyBasicAttr{19500, typeUp, 1700, 250, 50,
                                  speed1 * 1.1, false, "://images/enemyDemo2.png"};
    auto *c10 = new enemyBasicAttr{17500, typeDown, 1650, 200,
                                   100, speed1 * 1.1, false, "://images/enemyDemo2.png"};

    *enemyAttrList << a1 << a2 << a3 << a4 << a5 << a6 << a7 << b8 << b9 << c10;
    std::sort(enemyAttrList->begin(), enemyAttrList->end(),
              [](const enemyBasicAttr *a, const enemyBasicAttr *b) -> bool {
                  return a->birthTime < b->birthTime;
              });
}

void GameControl::readFriendInfo(const QString &mapName) {
    friendTotalNum = 6;
    friendAttrList = new QList<AbstractFriendAttr *>();

    //过渡代码， readFriendInfo 从文件读入
    AbstractFriendAttr *a[friendTotalNum];

    a[0] = new AbstractFriendAttr{1, 27, 878, 453, 69,
                                  2900, "://images/LavaFight.png",
                                  "://images/LavaMG.png",
                                  AbstractFriendObjects::SplashCaster};
    a[1] = new AbstractFriendAttr{1, 9, 831, 251, 93,
                                  1000, "://images/KroosFight.png",
                                  "://images/KroosMG.png",
                                  AbstractFriendObjects::Shooter};
    a[2] = new AbstractFriendAttr{1, 9, 831, 251, 93,
                                  1000, "://images/AdnachielFight.png",
                                  "://images/AdnachielMG.png",
                                  AbstractFriendObjects::Shooter};
    a[3] = new AbstractFriendAttr{2, 9, 1060, 235, 187,
                                  1050, "://images/fengFight.png",
                                  "://images/fengMG.png",
                                  AbstractFriendObjects::Vanguard};
    a[4] = new AbstractFriendAttr{3, 15, 1429, 236, 331,
                                  1200, "://images/bandianFight.png",
                                  "://images/bandianMG.png",
                                  AbstractFriendObjects::Defender};
    a[5] = new AbstractFriendAttr{1, 15, 907, 257, 81,
                                  2850, "://images/anselFight.png",
                                  "://images/anselMG.png",
                                  AbstractFriendObjects::Healer};
    for (int i = 0; i < friendTotalNum; i++) *friendAttrList << a[i];
    std::sort(friendAttrList->begin(), friendAttrList->end(),
              [](const AbstractFriendAttr *a, const AbstractFriendAttr *b) -> bool {
                  return a->cost > b->cost;
              });
}

void GameControl::initEnemy() {
    for (int i = 0; i < enemyAttrList->size(); i++) {
        auto birthTimer = new QTimer();
        birthTimer->setSingleShot(true);
        connect(birthTimer, SIGNAL(timeout()), this, SLOT(newEnemy()));
        birthTimer->start(enemyAttrList->at(i)->birthTime);
    }
}

void GameControl::newEnemy() {
    auto enemy = new AbstractEnemy(*(enemyAttrList->at(curBirthEnemy)));
    scene->addItem(enemy);
    curBirthEnemy++;
}

void GameControl::initFriend() {
    double posX = viewWidth - gridSizeX;
    for (auto i: *friendAttrList) {
        AbstractFriendObjects *friendObj;
        switch (i->type) {
            case AbstractFriendObjects::Shooter:
                friendObj = new ShooterFriend(i->blockNum, i->cost,
                                              i->healthLimit,
                                              i->atk,
                                              i->def,
                                              i->atkInterval,
                                              i->appearanceFileName,
                                              i->MugShotFileName);
                break;
            case AbstractFriendObjects::Defender:
                friendObj = new DefenderFriend(i->blockNum, i->cost,
                                               i->healthLimit,
                                               i->atk,
                                               i->def,
                                               i->atkInterval,
                                               i->appearanceFileName,
                                               i->MugShotFileName);
                break;
            case AbstractFriendObjects::Vanguard:
                friendObj = new VanguardFriend(i->blockNum, i->cost,
                                               i->healthLimit,
                                               i->atk,
                                               i->def,
                                               i->atkInterval,
                                               i->appearanceFileName,
                                               i->MugShotFileName);
                break;
            case AbstractFriendObjects::Healer:
                friendObj = new HealerFriend(i->blockNum, i->cost,
                                             i->healthLimit,
                                             i->atk,
                                             i->def,
                                             i->atkInterval,
                                             i->appearanceFileName,
                                             i->MugShotFileName);
                break;
            case AbstractFriendObjects::SplashCaster:
                friendObj = new SplashCasterFriend(i->blockNum, i->cost,
                                                   i->healthLimit,
                                                   i->atk,
                                                   i->def,
                                                   i->atkInterval,
                                                   i->appearanceFileName,
                                                   i->MugShotFileName);
                break;
        }
        friendObj->setPos(posX, viewHeight - gridSizeY);
        posX -= gridSizeX;
        scene->addItem(friendObj);
    }
}

void GameControl::mousePressEvent(QMouseEvent *event) {
    qDebug() << "pressed";
    if (cursor) {
        for (auto i: gridList) {
            if (((cursor->getType() == AbstractFriendObjects::Vanguard ||
                  cursor->getType() == AbstractFriendObjects::Defender) &&
                 i->getType() == AbstractGrid::groundGrid && i->getDeployAbility()) ||
                ((cursor->getType() == AbstractFriendObjects::Healer ||
                  cursor->getType() == AbstractFriendObjects::Shooter ||
                  cursor->getType() == AbstractFriendObjects::SplashCaster) &&
                 i->getType() == AbstractGrid::airGrid && i->getDeployAbility()))
                if (event->pos().x() > i->pos().x() &&
                    event->pos().x() < i->pos().x() + gridSizeX &&
                    event->pos().y() > i->pos().y() &&
                    event->pos().y() < i->pos().y() + gridSizeY) {
                    cursor->intoFight(i->pos());
                    setAirGridGreen(false);
                    setGroundGridGreen(false);
                    cursor = nullptr;
                    gameStatus = normal;
                    qDebug() << "into normal";
                    break;
                }
        }
    }
    QGraphicsView::mousePressEvent(event);
}


void GameControl::mouseMoveEvent(QMouseEvent *event) {
    if (cursor) cursor->setPos(event->pos());
    QGraphicsView::mouseMoveEvent(event);
}

void GameControl::setAirGridGreen(bool g) {
    if (g)
        for (auto i: gridList) {
            if (i->getType() == AbstractGrid::airGrid && i->getDeployAbility()) i->setGreen();
        }
    else
        for (auto i: gridList) {
            if (i->getType() == AbstractGrid::airGrid && i->getDeployAbility()) i->setDefault();
        }
}

void GameControl::setGroundGridGreen(bool g) {
    if (g)
        for (auto i: gridList) {
            if (i->getType() == AbstractGrid::groundGrid && i->getDeployAbility()) i->setGreen();
        }
    else
        for (auto i: gridList) {
            if (i->getType() == AbstractGrid::groundGrid && i->getDeployAbility()) i->setDefault();
        }
}