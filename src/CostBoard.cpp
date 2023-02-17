//
// Created by Chris on 2023/2/9.
//

#include "CostBoard.h"
#include "GameControl.h"

extern GameControl *game;

CostBoard::CostBoard(int initCost) : curCost(initCost),
                                     warningShowing(false) {
    setPlainText("cost: " + QString::number(initCost));
    setFont(QFont("Consolas", 28));
    connect(&costTimer, &QTimer::timeout,
            [this]() {
                addCost(1);
            });
    costTimer.start(500);

    costBoardBg = new QGraphicsRectItem(0, 0, 230, 60, this);
    costBoardBg->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    costBoardBg->setPen(QPen(Qt::NoPen));
    costBoardBg->setOpacity(0.2);
}

bool CostBoard::minusCost(int cost) {
    if (curCost < cost) {
        if (!warningShowing) {
            game->warning->setPlainText("Insufficient Cost");
            scene()->addItem(game->warning);
            warningShowing = true;
            warningTimer.start(1500);
            connect(&warningTimer, &QTimer::timeout,
                    this, &CostBoard::removeWarning,
                    Qt::UniqueConnection);
        }
        return false;
    }
    curCost -= cost;
    setPlainText("cost: " + QString::number(curCost));
    return true;
}

void CostBoard::addCost(int cost) {
    if (curCost + cost >= 99) {
        curCost = 99;
        setPlainText("cost: " + QString::number(curCost));
        return;
    }
    curCost += cost;
    setPlainText("cost: " + QString::number(curCost));
}

void CostBoard::removeWarning() {
    scene()->removeItem(game->warning);
    warningShowing = false;
    warningTimer.stop();
}
