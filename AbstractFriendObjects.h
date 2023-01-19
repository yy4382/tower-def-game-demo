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
    AbstractFriendObjects(int blockNum_, double healthLimit_, double atk_, double def_,
                          double atkInterval_, QGraphicsRectItem *parent, QString appearFileName,
                          QString MSFileName);

private:
    int blockNum;
    double health, healthLimit, atk, def, atkInterval;
    QPointF location;
    QPoint gridLocation;
    QGraphicsPixmapItem appearance;
    QString appearanceFileName;
    QString MugShotFileName;
    QGraphicsPolygonItem attackArea;

};


#endif //TOWERDEF_ABSTRACTFRIENDOBJECTS_H
