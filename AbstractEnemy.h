//
// Created by Chris on 2023/1/17.
//

#ifndef TOWERDEF_ABSTRACTENEMY_H
#define TOWERDEF_ABSTRACTENEMY_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QList>
#include <QPointF>
#include <QString>
#include <QTimer>
#include <QLineF>
#include <QtMath>
#include <utility>

struct enemyBasicAttr {
    enemyBasicAttr(int birthTime_, QList<QPointF> *path_,
                   double healthLimit_,
                   double atk_,
                   double def_,
                   double speed_,
                   bool isFlyingObject_,
                   QString appearance_
    ) : birthTime(birthTime_), path(*path_), healthLimit(healthLimit_), atk(atk_),
        def(def_), speed(speed_), isFlyingObject(isFlyingObject_),
        appearance(std::move(appearance_)) {};

    // basic attributes of an enemy
    int birthTime;
    QList<QPointF> path;
    double healthLimit, atk, def, speed;
    bool isFlyingObject;
    QString appearance;
};

class AbstractEnemy : public QObject, public QGraphicsRectItem {
Q_OBJECT
public:
    explicit AbstractEnemy(const enemyBasicAttr &);

    double distanceLeft();
    void beAttacked(double damage);
protected:
    QGraphicsPixmapItem *appearance;
    QString appearanceFileName;
private:
    enum dieType{hp_0,arriveAtDest};
    QList<QPointF> path;
    int pathIndex; //to which point in the path the enemy is approaching
    double healthLimit, health, atk, def, speed;
    bool isFlyingObject;
    QGraphicsRectItem *hpBar;
    QGraphicsRectItem *hpFullBar;
    void setHpBar();

    void die(dieType);

    QTimer *enemyMoveTimer;

    void setAppearance(bool ifFlipped);

public slots:

    void move();
};


#endif //TOWERDEF_ABSTRACTENEMY_H
