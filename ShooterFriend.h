//
// Created by Chris on 2023/2/2.
//

#ifndef TOWERDEF_SHOOTERFRIEND_H
#define TOWERDEF_SHOOTERFRIEND_H

#include "AbstractFriendObjects.h"

class ShooterFriend : public AbstractFriendObjects {
public:
    virtual friendType getType() { return Shooter; }

    ShooterFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                  double atkInterval, const QString &appearanceFileName,
                  const QString &msFileName, QGraphicsRectItem *parent = nullptr);
};


#endif //TOWERDEF_SHOOTERFRIEND_H
