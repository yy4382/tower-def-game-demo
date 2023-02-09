//
// Created by Chris on 2023/2/2.
//

#ifndef TOWERDEF_HEALERFRIEND_H
#define TOWERDEF_HEALERFRIEND_H
#include "AbstractFriendObjects.h"
class HealerFriend: public AbstractFriendObjects{
public:
    friendType getType() override { return Healer; }

    HealerFriend(int blockNum, int cost, double healthLimit, double atk, double def,
                 double atkInterval, const QString &appearFileName, const QString &msFileName,
                 QGraphicsRectItem *parent = nullptr);
protected:
    void attack(QGraphicsItem *target) override;
public slots:
    void acquireTarget() override;
};


#endif //TOWERDEF_HEALERFRIEND_H
