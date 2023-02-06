//
// Created by Chris on 2023/2/2.
//

#ifndef TOWERDEF_DEFENDERFRIEND_H
#define TOWERDEF_DEFENDERFRIEND_H

#include "GameControl.h"
#include "AbstractFriendObjects.h"

class DefenderFriend : public AbstractFriendObjects {
public:
    friendType getType() override { return Defender; }

    DefenderFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                   double atkInterval, const QString &appearanceFileName,
                   const QString &msFileName,QGraphicsRectItem *parent = nullptr);
};


#endif //TOWERDEF_DEFENDERFRIEND_H
