//
// Created by Chris on 2023/1/15.
//

#include "GameControl.h"

#include "ShooterFriend.h"
#include "DefenderFriend.h"
#include "VanguardFriend.h"
#include "HealerFriend.h"
#include "SplashCasterFriend.h"
#include "GroundGrid.h"
#include "GateGrid.h"
#include "AirGrid.h"

#include <QDebug>
#include <QFile>
#include <QBitArray>

GameControl::GameControl() : curBirthEnemy(0) {
    //从文件中读取
    readInfo(":/mapInfo/resources/testMap.txt");

    // 设置view
    setFixedSize(viewWidth, viewHeight);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create and set a scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, viewWidth, viewHeight);
    setScene(scene);

    // initial mouse events
    setMouseTracking(true);
    cursor = nullptr;
    gameStatus = normal;

    // 初始化背景格子、敌人、友方单位、记分牌和cost计算器
    initBackground();
    initEnemy();
    auto friendInit = new QTimer();
    friendInit->setSingleShot(true);
    connect(friendInit, SIGNAL(timeout()), this, SLOT(initFriend()));
    friendInit->start(10);
    connect(friendInit, &QTimer::timeout, [this]() {
        scoreBoard = new ScoreBoard;
        scene->addItem(scoreBoard);
        costBoard = new CostBoard(initCost);
        scene->addItem(costBoard);
        costBoard->setPos(scene->width() - 250, 0);
    });

    // 初始化warning
    warning = new QGraphicsTextItem;
    warning->setPos(0, marginUp + gridSizeY * rowY + 20);
    warning->setFont(QFont("Segoe UI", 25));
    warning->setDefaultTextColor(Qt::red);
}

void GameControl::initBackground() {
    //init grids
    double x = marginLeftAndRight, y = marginUp;
    int index = 0;
    for (int i = 0; i < rowY; i++) {
        for (int j = 0; j < rowX; j++) {
            if ((gridAttrList->at(index))->type != "null") {
                //Gate
                auto *grid = new GateGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                          (gridAttrList->at(index))->appearance,
                                          (gridAttrList->at(index))->greenAppearance,
                                          (gridAttrList->at(index))->type);
                scene->addItem(grid);
                gridList << grid;
            } else if ((gridAttrList->at(index))->ground == 1) {
                //ground grid
                auto *grid = new GroundGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                            (gridAttrList->at(index))->appearance,
                                            (gridAttrList->at(index))->greenAppearance,
                                            (gridAttrList->at(index))->deployability);
                scene->addItem(grid);
                gridList << grid;
            } else if ((gridAttrList->at(index))->ground == 0) {
                // air grid
                auto *grid = new AirGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                         (gridAttrList->at(index))->appearance,
                                         (gridAttrList->at(index))->greenAppearance,
                                         (gridAttrList->at(index))->deployability);
                scene->addItem(grid);
                gridList << grid;
            }
            x += gridSizeX;
            index++;
        }
        y += gridSizeY;
        x = 0;
    }

}


QPointF GameControl::indexToCoordinate(int x, int y) const {
    double xx = marginLeftAndRight, yy = marginUp;
    for (int i = 0; i < x; i++) xx += gridSizeX;
    for (int i = 0; i < y; i++) yy += gridSizeY;
    return {xx, yy};
}


void GameControl::initEnemy() {
    for (auto i: *enemyAttrList) {
        auto birthTimer = new QTimer();
        birthTimers.append(birthTimer);
        birthTimer->setSingleShot(true);
        connect(birthTimer, SIGNAL(timeout()), this, SLOT(newEnemy()));
        birthTimer->start(i->birthTime);
    }
}

void GameControl::newEnemy() {
    auto enemy = new AbstractEnemy(*(enemyAttrList->at(curBirthEnemy)));
    enemyList.append(enemy);
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
            case AbstractFriendObjects::null:
                break;
        }
        friendObj->setPos(posX, viewHeight - gridSizeY);
        friendObj->location = QPointF(posX, viewHeight - gridSizeY);
        posX -= gridSizeX;
        scene->addItem(friendObj);
    }
}

void GameControl::mousePressEvent(QMouseEvent *event) {
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
                    gameStatus = selectDir;
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

void GameControl::readInfo(const QString &mapName) {
    //打开文件
    QFile qFile{mapName};
    qFile.open(QIODevice::ReadOnly);

    //读取地图基本信息
    QByteArray byteArrayRead = qFile.readLine();
    QList<QByteArray> listRead = byteArrayRead.split(' ');
    viewWidth = listRead[0].trimmed().toInt();
    viewHeight = listRead[1].trimmed().toInt();

    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    gridSizeX = listRead[0].trimmed().toInt();
    gridSizeY = listRead[1].trimmed().toInt();

    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    marginLeftAndRight = listRead[0].trimmed().toInt();
    marginUp = listRead[1].trimmed().toInt();

    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    rowX = listRead[0].trimmed().toInt();
    rowY = listRead[1].trimmed().toInt();

    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    mapHp = listRead[0].trimmed().toInt();
    initCost = listRead[1].trimmed().toInt();

    //读取格子种类
    auto gridAttrType = new QList<GridAttr *>;
    while (true) {
        byteArrayRead = qFile.readLine();
        if (byteArrayRead == "\r\n") continue;
        byteArrayRead = qFile.readLine();
        QString appr = QString(byteArrayRead);
        appr.chop(2);

        byteArrayRead = qFile.readLine();
        QString greenAppr = QString(byteArrayRead);
        greenAppr.chop(2);

        byteArrayRead = qFile.readLine();
        listRead = byteArrayRead.split(' ');
        bool deploy = listRead[0].trimmed().toInt();
        bool ground = listRead[1].trimmed().toInt();

        byteArrayRead = qFile.readLine();
        if (byteArrayRead.trimmed() == "#end") {
            auto temp = new GridAttr(appr, greenAppr, deploy, ground);
            *gridAttrType << temp;
        } else {
            auto temp = new GridAttr(appr, greenAppr, deploy, ground, QString(byteArrayRead));
            *gridAttrType << temp;
            byteArrayRead = qFile.readLine();
        }

        byteArrayRead = qFile.readLine();
        if (byteArrayRead.trimmed() == "#endGridAttr") break;
    }

    //读取格子位置信息
    gridAttrList = new QList<GridAttr *>;
    byteArrayRead = qFile.readLine();
    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    for (const auto &i: listRead) {
        int typeIndex = i.toInt();
        *gridAttrList << gridAttrType->at(typeIndex);
    }

    //敌人路径种类
    auto *pathType = new QList<QList<QPointF> *>;
    byteArrayRead = qFile.readLine();
    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    while (true) {
        auto index = new QList<QPointF>;
        for (int i = 0; i < listRead.size(); i += 2)
            index->append(indexToCoordinate(listRead[i].trimmed().toInt(),
                                            listRead[i + 1].trimmed().toInt()));
        *pathType << index;
        byteArrayRead = qFile.readLine();
        if (byteArrayRead.trimmed() == "#endPathAttr") break;
        listRead = byteArrayRead.split(' ');
    }

    qFile.readLine();
    byteArrayRead = qFile.readLine();
    double speed1 = byteArrayRead.trimmed().toDouble();
    qFile.readLine();

    //敌人种类
    struct enemyTypeAttr {
        double healthLimit{}, atk{}, def{}, speed{}, atkInterval{};
        bool fly{};
        QString appearance;
    };
    auto enemyTypeList = new QList<enemyTypeAttr *>;
    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    while (true) {
        auto tempEnemyType = new enemyTypeAttr;
        tempEnemyType->healthLimit = listRead[0].toDouble();
        tempEnemyType->atk = listRead[1].toDouble();
        tempEnemyType->def = listRead[2].toDouble();
        tempEnemyType->speed = listRead[3].toDouble();
        tempEnemyType->fly = listRead[4].toInt();
        tempEnemyType->appearance = QString(listRead[5]).left(QString(listRead[5]).length() - 2);
        *enemyTypeList << tempEnemyType;
        byteArrayRead = qFile.readLine();
        if (byteArrayRead.trimmed() == "#endEnemyType") break;
        listRead = byteArrayRead.split(' ');
    }

    //出怪列表
    enemyAttrList = new QList<enemyBasicAttr *>;
    qFile.readLine();
    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    while (true) {
        int typeIndex = listRead[0].trimmed().toInt();
        int pathIndex = listRead[1].trimmed().toInt();
        *enemyAttrList << new enemyBasicAttr{listRead[2].trimmed().toInt(), pathType->at(pathIndex),
                                             enemyTypeList->at(typeIndex)->healthLimit,
                                             enemyTypeList->at(typeIndex)->atk,
                                             enemyTypeList->at(typeIndex)->def,
                                             speed1 * enemyTypeList->at(typeIndex)->speed,
                                             enemyTypeList->at(typeIndex)->fly,
                                             enemyTypeList->at(typeIndex)->appearance};
        byteArrayRead = qFile.readLine();
        if (byteArrayRead.trimmed() == "#endEnemyInfo") break;
        listRead = byteArrayRead.split(' ');
    }
    std::sort(enemyAttrList->begin(), enemyAttrList->end(),
              [](const enemyBasicAttr *a, const enemyBasicAttr *b) -> bool {
                  return a->birthTime < b->birthTime;
              });
    enemyTotalNum = enemyAttrList->size();

    //友方单位
    friendAttrList = new QList<AbstractFriendAttr *>;
    qFile.readLine();
    byteArrayRead = qFile.readLine();
    listRead = byteArrayRead.split(' ');
    while (true) {
        *friendAttrList << new AbstractFriendAttr{listRead[0].toInt(),
                                                  listRead[1].toInt(),
                                                  listRead[2].toDouble(),
                                                  listRead[3].toDouble(),
                                                  listRead[4].toDouble(),
                                                  listRead[5].toDouble(),
                                                  QString(listRead[6]),
                                                  QString(listRead[7]),
                                                  AbstractFriendObjects::intToFriendType(
                                                          listRead[8].toInt())};
        byteArrayRead = qFile.readLine();
        if (byteArrayRead.trimmed() == "#endFriend") break;
        listRead = byteArrayRead.split(' ');
    }
    std::sort(friendAttrList->begin(), friendAttrList->end(),
              [](const AbstractFriendAttr *a, const AbstractFriendAttr *b) -> bool {
                  return a->cost > b->cost;
              });
}

void GameControl::gameOver(bool ifVictory) {
    for (auto i: scene->items()) scene->removeItem(i);
    for (auto i: friendList)delete i;
    for (auto i: birthTimers) delete i;
    for (auto i: enemyList) delete i;
    item = new QList<QGraphicsItem *>;
    *item = scene->items();
    auto *word = new QGraphicsTextItem;
    if (ifVictory)
        word->setPlainText("Victory");
    else word->setPlainText("Failed");
    word->setFont(QFont("times", 80));
    scene->addItem(word);
}

