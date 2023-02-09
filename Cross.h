//
// Created by Chris on 2023/2/8.
//

#ifndef TOWERDEF_CROSS_H
#define TOWERDEF_CROSS_H
#include <QGraphicsPixmapItem>

class Cross :public QGraphicsPixmapItem{
public:
    Cross(QGraphicsItem*);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QGraphicsItem* parentObj;
};


#endif //TOWERDEF_CROSS_H
