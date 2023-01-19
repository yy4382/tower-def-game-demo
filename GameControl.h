//
// Created by Chris on 2023/1/15.
//

#ifndef TOWERDEF_GAMECONTROL_H
#define TOWERDEF_GAMECONTROL_H

#include "AbstractGrid.h"
#include "GroundGrid.h"
#include "GateGrid.h"
#include "AbstractEnemy.h"
#include "AirGrid.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QObject>
#include <utility>
#include <QPointF>
#include <QTimer>

struct GridAttr {
    GridAttr(QString appr_, bool deploy_, bool ground_, QString type_ = "null")
            : appearance(std::move(appr_)),
              deployability(deploy_),
              ground(ground_),
              type(std::move(type_)) {}

    QString appearance;
    bool deployability;
    bool ground;
    QString type;
};

class GameControl : public QGraphicsView {
Q_OBJECT
public:
    GameControl();
    int gridSizeX;
    int gridSizeY;

private:
    QGraphicsScene *scene;
    //map related
    GridAttr *gridAttr;
    int rowX;
    int rowY;
    int marginLeftAndRight;
    int marginUp;
    int viewWidth;
    int viewHeight;


    //enemy related
    int enemyTotalNum;
    QList<enemyBasicAttr*> *enemyList;
    int curBirthEnemy;

    //read info
    void readMapInfo(const QString &mapName);

    void readEnemyInfo(const QString &mapName);

    QPointF indexToCoordinate(int x, int y) const;

    //initial
    void initBackground();

    void initEnemy();

public slots:
    void newEnemy();
};


#endif //TOWERDEF_GAMECONTROL_H
