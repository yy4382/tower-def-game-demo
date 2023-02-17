//
// Created by Chris on 2023/1/15.
//

#ifndef TOWERDEF_ABSTRACTGRID_H
#define TOWERDEF_ABSTRACTGRID_H

#include <QPointF>
#include <QGraphicsPixmapItem>
#include <utility>

struct GridAttr {
    GridAttr(QString appr_,
             QString greenAppr_,
             bool deploy_,
             bool ground_,
             QString type_ = "null")
            : appearance(std::move(appr_)),
              greenAppearance(std::move(greenAppr_)),
              deployability(deploy_),
              ground(ground_),
              type(std::move(type_)) {}

    QString appearance;
    QString greenAppearance;
    bool deployability;
    bool ground;
    QString type;
};

class AbstractGrid : public QGraphicsPixmapItem {
private:
    int sizeX;
    int sizeY;
    QPointF location;
    QPixmap appearance;
    QPixmap greenAppearance;
public:
    explicit AbstractGrid(const QPointF &_location,
                          int _sizeX,
                          int _sizeY,
                          const QString &appr,
                          const QString &greenAppr,
                          QGraphicsPixmapItem *parent = nullptr);

    enum gridType {
        airGrid, groundGrid, redGateGrid, blueGateGrid
    };

    virtual gridType getType() = 0;

    virtual bool getDeployAbility() { return false; }

    void setGreen();

    void setDefault();
};


#endif //TOWERDEF_ABSTRACTGRID_H
