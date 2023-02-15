//
// Created by Chris on 2023/2/8.
//

#ifndef TOWERDEF_SCOREBOARD_H
#define TOWERDEF_SCOREBOARD_H
#include <QGraphicsTextItem>
#include <QObject>

class ScoreBoard:  public QGraphicsTextItem {

public:
    ScoreBoard();
    void hpMinus();
    void killAdd();
private:
    int currentKill;
    int hpLeft;

};


#endif //TOWERDEF_SCOREBOARD_H
