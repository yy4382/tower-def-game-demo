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


private:
    int blockNum, cost;
    double health, healthLimit, atk, def, atkInterval;
    QPointF location;
    QPoint gridLocation;
    QGraphicsPixmapItem *appearance;
    QString appearanceFileName; //战斗照文件名
    QString MugShotFileName;    //大头照文件名
    QGraphicsPolygonItem *attackArea;
    appearanceMode mode;
    QGraphicsRectItem *hpBar;
    bool readyToAttack;
    void setHpBar();



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void showPointer();

    void setAppearance(appearanceMode);
    QVector<QPointF> *attackAreaAttr;
    virtual void attack(QGraphicsItem* target);
    QTimer friendTimer;
    QTimer attackTimer;

public slots:
    virtual void acquireTarget();

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
