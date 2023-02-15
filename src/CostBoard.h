//
// Created by Chris on 2023/2/9.
//

#ifndef TOWERDEF_COSTBOARD_H
#define TOWERDEF_COSTBOARD_H

#include <QGraphicsTextItem>
#include <QTimer>

class CostBoard : public QGraphicsTextItem {
Q_OBJECT
public:
    CostBoard(int initCost);

    bool minusCost(int cost);

    void addCost(int cost);
public slots:
    void removeWarning();

private:
    int curCost;
    QTimer costTimer;
    QGraphicsRectItem *costBoardBg;
    bool warningShowing;
    QTimer warningTimer;
};


#endif //TOWERDEF_COSTBOARD_H
