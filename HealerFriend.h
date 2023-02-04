//
// Created by Chris on 2023/2/2.
//

#ifndef TOWERDEF_HEALERFRIEND_H
#define TOWERDEF_HEALERFRIEND_H
#include "AbstractFriendObjects.h"
class HealerFriend: public AbstractFriendObjects{
public:
    virtual friendType getType() { return Healer; }

    HealerFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                 double atkInterval, const QString &appearFileName, const QString &msFileName,
                 QGraphicsRectItem *parent = nullptr) : AbstractFriendObjects(blockNum, cost, healthLimit,
                                                                    atk, def, atkInterval,
                                                                    appearFileName, msFileName,
                                                                    parent) {}
};


#endif //TOWERDEF_HEALERFRIEND_H
