//
// Created by Chris on 2023/2/2.
//

#ifndef TOWERDEF_VANGUARDFRIEND_H
#define TOWERDEF_VANGUARDFRIEND_H

#include "AbstractFriendObjects.h"
class VanguardFriend: public AbstractFriendObjects {
public:
    virtual friendType getType() { return Vanguard; }

    VanguardFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                   double atkInterval, const QString &appearFileName, const QString &msFileName,
                   QGraphicsRectItem *parent = nullptr);
};


#endif //TOWERDEF_VANGUARDFRIEND_H
