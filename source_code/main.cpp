#include <QApplication>

#include "view/menu_game.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  TreasureHunt_2_0::ViewMenuGame main_window;
  main_window.show();
  return a.exec();
}
