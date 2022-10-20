#include <QApplication>

#include "view/playing_field_game.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  TreasureHunt_2_0::ViewPlayingField w(2);
  w.show();
  return a.exec();
}
