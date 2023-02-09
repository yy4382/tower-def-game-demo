//
// Created by Chris on 2023/1/19.
// 此类是所有我方单位的基类
//

#ifndef TOWERDEF_ABSTRACTFRIENDOBJECTS_H
#define TOWERDEF_ABSTRACTFRIENDOBJECTS_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QPixmap>
#include <QPointF>
#include <QPoint>
#include <QPolygonF>
#include <utility>
#include <QTimer>
#include "AbstractEnemy.h"
#include "Tick.h"
#include "Cross.h"

class AbstractFriendObjects : public QObject, public QGraphicsRectItem {
Q_OBJECT


public:
    enum friendType {
        null = 0, Shooter, Defender, Healer, Vanguard, SplashCaster
    };
    enum appearanceMode {
        MugShot, Fight, FlippedFight, underCursor
    };
    enum directions {
        right, left, up, down
    };


    AbstractFriendObjects(int blockNum_, int cost_, double healthLimit_, double atk_, double def_,
                          double atkInterval_, QString appearFileName, QString MSFileName,
                          QGraphicsRectItem *parent = nullptr);

    virtual friendType getType() { return null; }

    virtual void intoFight(QPointF position);

    void setDir(directions dir);

    static friendType intToFriendType(int type);

    virtual void beAttacked(double damage);

//    double posX;
    QPointF location;

    QGraphicsPolygonItem *getAttackArea() { return attackArea; }

    Tick *tick{};
    Cross *cross{};

    virtual void die();
    double getHealthPercent(){return health/healthLimit;}
    appearanceMode getMode(){return mode;}
    void beHealed(double damage);

private:


    QGraphicsPixmapItem *appearance;
    QString appearanceFileName; //战斗照文件名
    QString MugShotFileName;    //大头照文件名

    appearanceMode mode;
    QGraphicsRectItem *hpBar{};
    QGraphicsRectItem *hpFullBar{};

    void initHpBar();

    void setHpBar();


protected:
    const int blockNum, cost;
    const double healthLimit, atk, def, atkInterval;
    double health;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void showPointer();

    void setAppearance(appearanceMode);

    QVector<QPointF> *attackAreaAttr{};

    virtual void attack(QGraphicsItem *target);

    QTimer friendTimer;
    QTimer attackTimer;
    QList<AbstractEnemy *> blockList;
    QGraphicsPolygonItem *attackArea{};

    bool readyToAttack;
public slots:

    virtual void acquireTarget();

signals:

    void dieSignal();

};

struct AbstractFriendAttr {
    AbstractFriendAttr(int blockNum_, int cost_, double healthLimit_,
                       double atk_, double def_, double atkInterval_, QString appearance_,
                       QString MGName, AbstractFriendObjects::friendType type_) :
            blockNum(blockNum_), cost(cost_),
            healthLimit(healthLimit_),
            atk(atk_),
            def(def_),
            atkInterval(atkInterval_),
            appearanceFileName(std::move(appearance_)),
            MugShotFileName(std::move(MGName)),
            type(type_) {}

    int blockNum, cost;
    double healthLimit, atk, def, atkInterval;
    QString appearanceFileName, MugShotFileName;
    AbstractFriendObjects::friendType type;
};

#endif //TOWERDEF_ABSTRACTFRIENDOBJECTS_H
