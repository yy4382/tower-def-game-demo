//
// Created by Chris on 2023/1/19.
//

#include "AbstractFriendObjects.h"

#include <utility>
#include "GameControl.h"

extern GameControl *game;

AbstractFriendObjects::AbstractFriendObjects(int blockNum_, double healthLimit_, double atk_,
                                             double def_, double atkInterval_,
                                             QGraphicsRectItem *parent, QString appearFileName,
                                             QString MSFileName) : QGraphicsRectItem(parent),
                                                                   healthLimit(healthLimit_),
                                                                   health(healthLimit_),
                                                                   atk(atk_), def(def_),
                                                                   atkInterval(atkInterval_),
                                                                   blockNum(blockNum_),
                                                                   appearanceFileName(std::move(
                                                                           appearFileName)),
                                                                   MugShotFileName(
                                                                           std::move(MSFileName)) {

}
