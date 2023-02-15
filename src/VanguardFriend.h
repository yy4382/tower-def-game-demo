//
// Created by Chris on 2023/2/2.
//

#ifndef TOWERDEF_VANGUARDFRIEND_H
#define TOWERDEF_VANGUARDFRIEND_H

#include "AbstractFriendObjects.h"

class VanguardFriend : public AbstractFriendObjects {
Q_OBJECT
public:
    virtual friendType getType() { return Vanguard; }

    VanguardFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                   double atkInterval, const QString &appearFileName, const QString &msFileName,
                   QGraphicsRectItem *parent = nullptr);

public slots:

    void addCostStart() {
        addCostTimer.start(3000);
    }

    void addCostStop() {
        addCostTimer.stop();
    }

    void addCost();

private:
    QTimer addCostTimer;
};


#endif //TOWERDEF_VANGUARDFRIEND_H
