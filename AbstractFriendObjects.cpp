//
// Created by Chris on 2023/1/19.
//

#include "AbstractFriendObjects.h"
#include <utility>
#include "GameControl.h"
#include <QDebug>

extern GameControl *game;


AbstractFriendObjects::AbstractFriendObjects(int blockNum_, int cost_, double healthLimit_,
                                             double atk_, double def_,
                                             double atkInterval_, QString appearFileName,
                                             QString MSFileName,
                                             QGraphicsRectItem *parent) : QGraphicsRectItem(parent),
                                                                   healthLimit(healthLimit_),
                                                                   health(healthLimit_),
                                                                   atk(atk_), def(def_),
                                                                   atkInterval(atkInterval_),
                                                                   blockNum(blockNum_), cost(cost_),
                                                                   appearanceFileName(std::move(appearFileName)),
                                                                   MugShotFileName(
                                                                           std::move(MSFileName)) {
    setPen(QPen(Qt::NoPen));
    //初始化图片
    appearance = new QGraphicsPixmapItem(this);
    setAppearance(MugShot);
    //设置碰撞箱大小
    setRect(0, 0, game->gridSizeX, game->gridSizeY);

}

void AbstractFriendObjects::setAppearance(appearanceMode a) {
    if (a == MugShot) {
        QPixmap look = QPixmap{MugShotFileName}.scaledToWidth(game->gridSizeX);
        appearance->setPixmap(look);
        mode = MugShot;
    } else if ( a == Fight){
        QPixmap look = QPixmap{appearanceFileName}.scaledToWidth(game->gridSizeX);
        appearance->setPixmap(look);
        appearance->setOffset(0,-look.height()+game->gridSizeY*0.8);
        mode = Fight;
    } else if ( a == FlippedFight){
        QPixmap look = QPixmap{appearanceFileName}.scaledToWidth(game->gridSizeX).transformed(QTransform().scale(-1,1));
        appearance->setPixmap(look);
        mode = Fight;
    } else if (a == underCursor){
        QPixmap look = QPixmap{appearanceFileName}.scaledToWidth(game->gridSizeX);
        appearance->setPixmap(look);
        appearance->setOffset(-game->gridSizeX/2, -game->gridSizeY/2);
        mode = underCursor;
    }
}

void AbstractFriendObjects::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug()<<"pressed2";
    if(game->gameStatus == GameControl::normal && mode == MugShot) {
        game->cursor = this;
        setAppearance(underCursor);
        game->gameStatus = GameControl::build;
        if(getType() == Defender || getType() == Vanguard) game->setGroundGridGreen(true);
        else game->setAirGridGreen(true);
    }
}

void AbstractFriendObjects::intoFight(QPointF position) {
    setPos(position);
    setAppearance(Fight);
}

