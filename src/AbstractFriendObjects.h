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
    //队员的职业
    enum friendType {
        null, Shooter, Defender, Healer, Vanguard, SplashCaster
        //未设置，狙击，重装，医疗，先锋，群攻术士
    };

    //获得队员类型
    virtual friendType getType() { return null; }

    //把数转换为队员类型
    static friendType intToFriendType(int type);


    //队员目前的状态
    enum appearanceMode {
        HeadShot, Fight, FlippedFight, underCursor
        //大头照（在待部署区中），战斗，战斗（翻转），正被拖动
    };

    //获取队员状态
    appearanceMode getMode() { return mode; }


    //方向
    enum directions {
        right, left, up, down
    };

    //设置方向
    void setDir(directions dir);


    AbstractFriendObjects(int blockNum,
                          int cost,
                          double healthLimit,
                          double atk,
                          double def,
                          double atkInterval,
                          QString appearanceFileName,
                          QString HSFileName,
                          QGraphicsRectItem *parent = nullptr);

    //进入战斗（已选定位置），显示箭头
    virtual void intoFight(QPointF position);

    //被攻击
    virtual void beAttacked(double damage);

    //被治疗
    void beHealed(double damage);

    //目前位置
    QPointF location;

    //返回攻击范围
    QGraphicsPolygonItem *getAttackArea() { return attackArea; }

    //获取生命值百分比
    double getHealthPercent() const { return health / healthLimit; }

    //勾对象
    Tick *tick{};
    //叉对象
    Cross *cross{};

    //死亡
    virtual void die(bool beKilled);

private:

    //外表对象
    QGraphicsPixmapItem *appearance;
    //战斗照文件名
    QString appearanceFileName;
    //大头照文件名
    QString headShotFileName;
    //目前状态
    appearanceMode mode;


    //血条相关
    //正常血条
    QGraphicsRectItem *hpBar{};
    //满血血条（一个框子）
    QGraphicsRectItem *hpFullBar{};

    //初始化血条
    void initHpBar();

    //修改血条
    void setHpBar();


protected:
    //基本属性
    int blockNum, cost;
    double healthLimit, atk, def, atkInterval;
    double health;

    //鼠标点击活动
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    //显示箭头
    void showPointer();

    //设置外观
    void setAppearance(appearanceMode);


    //计时器
    QTimer friendTimer;
    QTimer attackTimer;

    //被阻挡的对象列表
    QList<AbstractEnemy *> blockList;

    //更新阻挡列表
    void renewBlockList();

    //攻击范围属性
    QVector<QPointF> *attackAreaAttr{};
    //攻击范围对象
    QGraphicsPolygonItem *attackArea{};
    //是否可以攻击（攻击间隔已过）
    bool readyToAttack;

    virtual void acquireTarget();

    //攻击
    virtual void attack(QGraphicsItem *target);


    //headshot 状态下的cost显示
    QGraphicsRectItem *costBg;//背景
    QGraphicsTextItem *costDisplay;

    void showCost();

    void hideCost();


public slots:


    //每一个行动周期的行动
    void friendAction();

signals:

    void dieSignal();

    void fightStart();

    void fightStop();

};

struct AbstractFriendAttr {
    AbstractFriendAttr(int blockNum,
                       int cost,
                       double healthLimit,
                       double atk,
                       double def,
                       double atkInterval,
                       QString appearance,
                       QString HSName,
                       AbstractFriendObjects::friendType type) :
            blockNum(blockNum),
            cost(cost),
            healthLimit(healthLimit),
            atk(atk),
            def(def),
            atkInterval(atkInterval),
            appearanceFileName(std::move(appearance)),
            HeadShotFileName(std::move(HSName)),
            type(type) {}

    int blockNum, cost;
    double healthLimit, atk, def, atkInterval;
    QString appearanceFileName, HeadShotFileName;
    AbstractFriendObjects::friendType type;
};

#endif //TOWERDEF_ABSTRACTFRIENDOBJECTS_H
