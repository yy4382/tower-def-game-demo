//
// Created by Chris on 2023/1/19.
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


class AbstractFriendObjects : public QGraphicsRectItem {
public:
    enum friendType {
        Shooter = 1, Defender = 2, Healer = 3, Vanguard = 4, SplashCaster = 5, null = 0
    };
    enum appearanceMode {
        MugShot, Fight, FlippedFight, underCursor
    };

    AbstractFriendObjects(int blockNum_, int cost_, double healthLimit_, double atk_, double def_,
                          double atkInterval_, QString appearFileName, QString MSFileName,
                          QGraphicsRectItem *parent = nullptr);

    virtual friendType getType() { return null; }
    virtual void intoFight(QPointF position);
private:
    int blockNum, cost;
    double health, healthLimit, atk, def, atkInterval;
    QPointF location;
    QPoint gridLocation;
    QGraphicsPixmapItem *appearance;
    QString appearanceFileName;
    QString MugShotFileName;
    QGraphicsPolygonItem attackArea;
    appearanceMode mode;

    void setAppearance(appearanceMode);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
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
            appearanceFileName(appearance_),
            MugShotFileName(MGName),
            type(type_) {}

    int blockNum, cost;
    double healthLimit, atk, def, atkInterval;
    QString appearanceFileName, MugShotFileName;
    AbstractFriendObjects::friendType type;
};

#endif //TOWERDEF_ABSTRACTFRIENDOBJECTS_H
