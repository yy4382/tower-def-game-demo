#include <QApplication>
#include <QPushButton>
#include "GameControl.h"

GameControl *game;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    game = new GameControl();
    game->show();
    return QApplication::exec();
}
