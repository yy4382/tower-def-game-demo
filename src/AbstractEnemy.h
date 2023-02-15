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
private:
    enum dieType {
        hp_0, arriveAtDest
    };
public:
    explicit AbstractEnemy(const enemyBasicAttr &);

    double distanceLeft();

    void beAttacked(double damage);

    void blocked(QGraphicsRectItem *friendObj);;

    void free() {
        enemyMoveTimer->start(20);
        isBlocked = nullptr;
        enemyAttackTimer.stop();
    }

    bool getBlocked() { return (bool) isBlocked; };

    QGraphicsPixmapItem *appearance;

    void die(dieType);
    void beHealed(double damage);

signals:
    void dieSignal();

protected:

    QString appearanceFileName;
private:
    QList<QPointF> path;
    int pathIndex; //to which point in the path the enemy is approaching
    double healthLimit, health, atk, def, speed, atkInterval;
    bool isFlyingObject;
    QGraphicsRectItem *hpBar;
    QGraphicsRectItem *hpFullBar;
    QGraphicsRectItem *isBlocked;

    void initHpBar();

    void setHpBar();


    QTimer *enemyMoveTimer;
    QTimer enemyAttackTimer;

    void setAppearance(bool ifFlipped);

public slots:

    void move();

    void attack();
};


#endif //TOWERDEF_ABSTRACTENEMY_H
