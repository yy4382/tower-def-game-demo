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

//一个包含了敌人基本数据的结构体，用来给AbstractEnemy类初始化
struct enemyBasicAttr {
    enemyBasicAttr(int birthTime,
                   QList<QPointF> *path,
                   double healthLimit,
                   double atk,
                   double def,
                   double speed,
                   bool isFlyingObject,
                   QString appearance
    ) : birthTime(birthTime),
        path(*path),
        healthLimit(healthLimit),
        atk(atk),
        def(def),
        speed(speed),
        isFlyingObject(isFlyingObject),
        appearance(std::move(appearance)) {};

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
    //敌人死亡类型，分别为因为hp归零而死和到达目的地而死
    enum dieType {
        hp_0, arriveAtDest
    };

public:
    explicit AbstractEnemy(const enemyBasicAttr &);

    //返回离蓝门的距离
    double distanceLeft();

    //被攻击时触发此函数，顺带检查是否死亡
    void beAttacked(double damage);

    //被治疗时触发
    void beHealed(double damage);

    //被阻挡时触发此函数
    void blocked(QGraphicsRectItem *friendObj);;

    //不再被阻挡时触发此函数
    void free() {
        enemyMoveTimer->start(20);
        isBlocked = nullptr;
        enemyAttackTimer.stop();
    }

    //返回是否被阻挡
    bool getBlocked() { return (bool) isBlocked; };

signals:

    // 死亡时，发送此信号
    void dieSignal();


private:
    // 外表图片的文件名
    QString appearanceFileName;
    // 外表图片对象
    QGraphicsPixmapItem *appearance;

    //设置外表
    void setAppearance(bool ifFlipped);

    //路径相关
    //需要经过的路径点
    QList<QPointF> path;
    //现在正要去的路径点的index
    int pathIndex;

    //基本属性
    double healthLimit, health, atk, def, speed, atkInterval;
    bool isFlyingObject;

    //血条相关
    //正常血条
    QGraphicsRectItem *hpBar;
    //满血血条（一个框子）
    QGraphicsRectItem *hpFullBar;

    //初始化血条
    void initHpBar();

    //修改血条
    void setHpBar();

    //死亡，参数为为何死亡
    void die(dieType);

    //阻挡指针，未阻挡时为nullptr，被阻挡时指向
    QGraphicsRectItem *isBlocked;

    //敌人计时器
    QTimer *enemyMoveTimer;
    QTimer enemyAttackTimer;

public slots:

    //敌人移动
    void move();

    //敌人攻击
    void attack();
};


#endif //TOWERDEF_ABSTRACTENEMY_H
