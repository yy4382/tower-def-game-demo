//
// Created by Chris on 2023/2/8.
//

#include "ScoreBoard.h"
#include "GameControl.h"

extern GameControl *game;

ScoreBoard::ScoreBoard() : currentKill(0), hpLeft(game->mapHealth()) {
    setPlainText(QString("Score: 0/") + QString::number(game->enemySum()) + " Health: " +
                 QString::number(hpLeft) + QString("/") + QString::number(hpLeft));
    setFont(QFont("times", 28));
}

void ScoreBoard::hpMinus() {
    hpLeft--;
    if (hpLeft <= 0) {
        game->gameOver(false);
        delete this;
        return;
    }
    killAdd();
}

void ScoreBoard::killAdd() {
    currentKill++;
    setPlainText(
            "Score: " + QString::number(currentKill) + "/" + QString::number(game->enemySum()) +
            " Health: " + QString::number(hpLeft) + "/" + QString::number(game->mapHealth()));
    if (currentKill == game->enemySum()) {
        game->gameOver(true);
        delete this;
    }
}
