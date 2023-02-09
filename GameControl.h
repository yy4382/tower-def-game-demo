//
// Created by Chris on 2023/1/15.
//

#ifndef TOWERDEF_GAMECONTROL_H
#define TOWERDEF_GAMECONTROL_H


#include "AbstractGrid.h"
#include "AbstractEnemy.h"
#include "AbstractFriendObjects.h"

#include "Pointer.h"
#include "ScoreBoard.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>
#include <utility>
#include <QPointF>
#include <QTimer>
#include <QMouseEvent>
#include <QLabel>

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
    ScoreBoard *scoreBoard;

    int enemySum() { return enemyTotalNum; }
    int mapHealth() {return mapHp;}
    void gameOver(bool ifVictory);

private:
    QGraphicsScene *scene;
    //map related
    int rowX;
    int rowY;
    int marginLeftAndRight;
    int marginUp;
    int viewWidth;
    int viewHeight;
    int mapHp;
    QList<GridAttr *> *gridAttrList;
    QList<QGraphicsItem*> *item;

    //friendObject related
    int friendTotalNum;
    QList<AbstractFriendAttr *> *friendAttrList;

    //enemy related
    int enemyTotalNum;
    QList<enemyBasicAttr *> *enemyAttrList;
    int curBirthEnemy;

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
