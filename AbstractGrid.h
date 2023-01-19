//
// Created by Chris on 2023/1/15.
//

#ifndef TOWERDEF_ABSTRACTGRID_H
#define TOWERDEF_ABSTRACTGRID_H

#include <QPointF>
#include <QGraphicsPixmapItem>

class AbstractGrid : public QGraphicsPixmapItem {
private:
    int sizeX;
    int sizeY;
    QPointF location;
    QPixmap appearance;
public:
    explicit AbstractGrid(const QPointF &_location, int _sizeX, int _sizeY, const QString &appr,
                          QGraphicsPixmapItem *parent = nullptr);
};


#endif //TOWERDEF_ABSTRACTGRID_H
