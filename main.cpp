#include <QApplication>

#include "view_playing_field.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  InSearchOfTreasure_2_0::ViewPlayingField w(3);
  w.show();

//  InSearchOfTreasure::ViewGame w;
//  w.show();
  return a.exec();
}
