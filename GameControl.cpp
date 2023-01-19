//
// Created by Chris on 2023/1/15.
//

#include "GameControl.h"
#include <QDebug>

GameControl::GameControl() : curBirthEnemy(0) {
    readMapInfo("testMap");
    readEnemyInfo("testMap");

    //set view rect
    setFixedSize(viewWidth, viewHeight);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // create and set a scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, viewWidth, viewHeight);
    setScene(scene);

    //draw the map
    initBackground();

    //initial Enemy
    initEnemy();
}

void GameControl::initBackground() {
    //init grids
    double x = marginLeftAndRight, y = marginUp;
    int index = 0;
    for (int i = 0; i < rowY; i++) {
        for (int j = 0; j < rowX; j++) {
            if ((gridAttr + index)->type != "null") {
                //Gate
                auto *grid = new GateGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                          (gridAttr + index)->appearance, (gridAttr + index)->type);
                scene->addItem(grid);
            } else if ((gridAttr + index)->ground == 1) {
                //ground grid
                auto *grid = new GroundGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                            (gridAttr + index)->appearance,
                                            (gridAttr + index)->deployability);
                scene->addItem(grid);
            } else if ((gridAttr + index)->ground == 0) {
                // air grid
                auto *grid = new AirGrid(gridSizeX, gridSizeY, QPointF(x, y),
                                         (gridAttr + index)->appearance,
                                         (gridAttr + index)->deployability);
                scene->addItem(grid);
            }
            x += gridSizeX;
            index++;
        }
        y += gridSizeY;
        x = 0;
    }

    //init pause btn
    auto *pauseBtn = new QGraphicsPixmapItem(
            QPixmap("://images/pauseBtn.png").scaled(marginUp - 10, marginUp - 10));
    scene->addItem(pauseBtn);
    pauseBtn->setPos(QPointF(viewWidth - marginUp - 15, 5));


}

void GameControl::readMapInfo(const QString &mapName) {
    // read map info
    viewWidth = 1600;
    viewHeight = 900;
    gridSizeX = 160;
    gridSizeY = 160;
    marginLeftAndRight = 0;
    marginUp = 70;
    QString gd(":/images/DeployableGroundGrid1.png"),
            gu(":/images/UndeployableGroundGrid1.png"),
            rg(":/images/DeployableRedGateGrid1.png"),
            bg(":/images/DeployableBlueGateGrid1.png"),
            ad(":/images/DeployableAirGrid1.png"),
            au(":/images/UndeployableAirGrid1.png");
    GridAttr gda(gd, true, true),
            gua(gu, false, true),
            ada(ad, true, false),
            aua(au, false, false),
            rga(rg, false, false, "redGate"),
            bga(bg, false, false, "blueGate");
    rowX = 10;
    rowY = 5;
    gridAttr = new GridAttr[50]
            {
                    aua, ada, ada, ada, gda, gda, gda, gda, gua, rga,
                    aua, ada, gda, gda, gda, ada, ada, ada, aua, aua,
                    bga, gda, gda, aua, gda, gda, gda, gda, gua, rga,
                    aua, ada, gda, gda, gda, ada, ada, ada, aua, aua,
                    aua, ada, ada, ada, gda, gda, gda, gda, gua, rga
            };
}

QPointF GameControl::indexToCoordinate(int x, int y) const {
    double xx = marginLeftAndRight, yy = marginUp;
    for (int i = 0; i < x; i++) xx += gridSizeX;
    for (int i = 0; i < y; i++) yy += gridSizeY;
    return {xx, yy};
}

void GameControl::readEnemyInfo(const QString &mapName) {
// read enemy info
    enemyTotalNum = 10;
    QList<QPointF> typeUp, typeMiddleUp, typeMiddleDown, typeDown;
    typeUp << indexToCoordinate(9, 0) << indexToCoordinate(4, 0) << indexToCoordinate(4, 1)
           << indexToCoordinate(2, 1) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    typeMiddleUp << indexToCoordinate(9, 2) << indexToCoordinate(4, 2) << indexToCoordinate(4, 1)
                 << indexToCoordinate(2, 1) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    typeMiddleDown << indexToCoordinate(9, 2) << indexToCoordinate(4, 2) << indexToCoordinate(4, 3)
                   << indexToCoordinate(2, 3) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    typeDown << indexToCoordinate(9, 4) << indexToCoordinate(4, 4) << indexToCoordinate(4, 3)
             << indexToCoordinate(2, 3) << indexToCoordinate(2, 2) << indexToCoordinate(0, 2);
    double speed1 = 3;
    auto *a1 = new enemyBasicAttr{3000, typeUp, 1000, 185, 0,
                                  speed1, false,"://images/enemyDemo1.png"};
    auto *a2 = new enemyBasicAttr{6000, typeDown, 1000, 100, 0,
                                  speed1, false,"://images/enemyDemo1.png"};
    auto *a3 = new enemyBasicAttr{6500, typeDown, 1000, 100, 0,
                                  speed1, false,"://images/enemyDemo1.png"};
    auto *a4 = new enemyBasicAttr{9000, typeMiddleUp, 1000, 100,
                                  0,speed1, false,"://images/enemyDemo1.png"};
    auto *a5 = new enemyBasicAttr{11500, typeMiddleUp, 1000, 100,
                                  0, speed1, false,"://images/enemyDemo1.png"};
    auto *a6 = new enemyBasicAttr{9500, typeMiddleDown, 1000, 100,
                                  0, speed1, false,"://images/enemyDemo1.png"};
    auto *a7 = new enemyBasicAttr{12000, typeMiddleDown, 1000, 100,
                                  0, speed1, false,"://images/enemyDemo1.png"};
    auto *b8 = new enemyBasicAttr{16000, typeUp, 1700, 250, 50,
                                  speed1 * 1.1, false,"://images/enemyDemo2.png"};
    auto *b9 = new enemyBasicAttr{19500, typeUp, 1700, 250, 50,
                                  speed1 * 1.1, false,"://images/enemyDemo2.png"};
    auto *c10 = new enemyBasicAttr{17500, typeDown, 1650, 200,
                                   100, speed1 * 1.1, false,"://images/enemyDemo2.png"};
    enemyList = new QList<enemyBasicAttr*>();
    *enemyList << a1 << a2 << a3 << a4 << a5 << a6 << a7 << b8 << b9 << c10;
    std::sort(enemyList->begin(), enemyList->end(),
              [](const enemyBasicAttr *a, const enemyBasicAttr *b) -> bool {
                  return a->birthTime < b->birthTime;
              });
}

void GameControl::initEnemy() {
    for (int i = 0; i < enemyList->size(); i++) {
        auto birthTimer = new QTimer();
        birthTimer->setSingleShot(true);
        connect(birthTimer, SIGNAL(timeout()), this, SLOT(newEnemy()));
        birthTimer->start(enemyList->at(i)->birthTime);
    }
}

void GameControl::newEnemy() {
    auto enemy = new AbstractEnemy(*(enemyList->at(curBirthEnemy)));
    scene->addItem(enemy);
//    qDebug()<<"add to scene";
    curBirthEnemy++;
}
