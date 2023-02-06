//
// Created by Chris on 2023/1/15.
//

#ifndef TOWERDEF_GAMECONTROL_H
#define TOWERDEF_GAMECONTROL_H


#include "AbstractGrid.h"
#include "AbstractEnemy.h"
#include "AbstractFriendObjects.h"

#include "Pointer.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>
#include <utility>
#include <QPointF>
#include <QTimer>
#include <QMouseEvent>


class GameControl : public QGraphicsView {
Q_OBJECT


public:
    GameControl();

    enum GameStatus {
        normal, build, selectDir, showDetail, success
    };
    GameStatus gameStatus;
    int gridSizeX;
    int gridSizeY;
    AbstractFriendObjects *cursor;

    void setGroundGridGreen(bool g);

    void setAirGridGreen(bool g);

    QList<AbstractEnemy *> enemyList;
    QList<AbstractFriendObjects *> friendList;
    QList<AbstractGrid *> gridList;
    Pointer *pointers;

private:
    QGraphicsScene *scene;
    //map related
    int rowX;
    int rowY;
    int marginLeftAndRight;
    int marginUp;
    int viewWidth;
    int viewHeight;
    QList<GridAttr *> *gridAttrList;

    //friendObject related
    int friendTotalNum;
    QList<AbstractFriendAttr *> *friendAttrList;

    //enemy related
    int enemyTotalNum;
    QList<enemyBasicAttr *> *enemyAttrList;
    int curBirthEnemy;

    //read info
//    void readMapInfo(const QString &mapName);

//    void readEnemyInfo(const QString &mapName);

    void readFriendInfo(const QString &mapName);

    void readInfo(const QString &mapName);

    QPointF indexToCoordinate(int x, int y) const;

    //initializing
    void initBackground();

    void initEnemy();


public slots:

    void newEnemy();

    void initFriend();

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};


#endif //TOWERDEF_GAMECONTROL_H
